#pragma once

namespace NuclearEngine
{
	namespace Utilities 
	{
		template<typename T>
		class Delegate_base;

		template<typename RET, typename ...PARAMS>
		class Delegate_base<RET(PARAMS...)> {

		protected:

			using stub_type = RET(*)(void* this_ptr, PARAMS...);

			struct InvocationElement {
				InvocationElement() = default;
				InvocationElement(void* this_ptr, stub_type aStub) : object(this_ptr), stub(aStub) {}
				void Clone(InvocationElement& target) const {
					target.stub = stub;
					target.object = object;
				} //Clone
				bool operator ==(const InvocationElement& another) const {
					return another.stub == stub && another.object == object;
				} //==
				bool operator !=(const InvocationElement& another) const {
					return another.stub != stub || another.object != object;
				} //!=
				void* object = nullptr;
				stub_type stub = nullptr;
			}; //InvocationElement

		}; //class Delegate_base


		template <typename T> class Delegate;
		template <typename T> class multicast_Delegate;

		template<typename RET, typename ...PARAMS>
		class Delegate<RET(PARAMS...)> final : private Delegate_base<RET(PARAMS...)> {
		public:

			Delegate() = default;

			bool isNull() const { return invocation.stub == nullptr; }
			bool operator ==(void* ptr) const {
				return (ptr == nullptr) && this->isNull();
			} //operator ==
			bool operator !=(void* ptr) const {
				return (ptr != nullptr) || (!this->isNull());
			} //operator !=

			Delegate(const Delegate& another) { another.invocation.Clone(invocation); }

			template <typename LAMBDA>
			Delegate(const LAMBDA& lambda) {
				assign((void*)(&lambda), lambda_stub<LAMBDA>);
			} //Delegate

			Delegate& operator =(const Delegate& another) {
				another.invocation.Clone(invocation);
				return *this;
			} //operator =

			template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
			Delegate& operator =(const LAMBDA& instance) {
				assign((void*)(&instance), lambda_stub<LAMBDA>);
				return *this;
			} //operator =

			bool operator == (const Delegate& another) const { return invocation == another.invocation; }
			bool operator != (const Delegate& another) const { return invocation != another.invocation; }

			bool operator ==(const multicast_Delegate<RET(PARAMS...)>& another) const { return another == (*this); }
			bool operator !=(const multicast_Delegate<RET(PARAMS...)>& another) const { return another != (*this); }

			template <class T, RET(T::*TMethod)(PARAMS...)>
			static Delegate create(T* instance) {
				return Delegate(instance, method_stub<T, TMethod>);
			} //create

			template <class T, RET(T::*TMethod)(PARAMS...) const>
			static Delegate create(T const* instance) {
				return Delegate(const_cast<T*>(instance), const_method_stub<T, TMethod>);
			} //create

			template <RET(*TMethod)(PARAMS...)>
			static Delegate create() {
				return Delegate(nullptr, function_stub<TMethod>);
			} //create

			template <typename LAMBDA>
			static Delegate create(const LAMBDA & instance) {
				return Delegate((void*)(&instance), lambda_stub<LAMBDA>);
			} //create

			RET operator()(PARAMS... arg) const {
				return (*invocation.stub)(invocation.object, arg...);
			} //operator()

		private:

			Delegate(void* anObject, typename Delegate_base<RET(PARAMS...)>::stub_type aStub) {
				invocation.object = anObject;
				invocation.stub = aStub;
			} //Delegate

			void assign(void* anObject, typename Delegate_base<RET(PARAMS...)>::stub_type aStub) {
				this->invocation.object = anObject;
				this->invocation.stub = aStub;
			} //assign

			template <class T, RET(T::*TMethod)(PARAMS...)>
			static RET method_stub(void* this_ptr, PARAMS... params) {
				T* p = static_cast<T*>(this_ptr);
				return (p->*TMethod)(params...);
			} //method_stub

			template <class T, RET(T::*TMethod)(PARAMS...) const>
			static RET const_method_stub(void* this_ptr, PARAMS... params) {
				T* const p = static_cast<T*>(this_ptr);
				return (p->*TMethod)(params...);
			} //const_method_stub

			template <RET(*TMethod)(PARAMS...)>
			static RET function_stub(void* this_ptr, PARAMS... params) {
				return (TMethod)(params...);
			} //function_stub

			template <typename LAMBDA>
			static RET lambda_stub(void* this_ptr, PARAMS... arg) {
				LAMBDA* p = static_cast<LAMBDA*>(this_ptr);
				return (p->operator())(arg...);
			} //lambda_stub

			friend class multicast_Delegate<RET(PARAMS...)>;
			typename Delegate_base<RET(PARAMS...)>::InvocationElement invocation;

		}; //class Delegate

	}
}