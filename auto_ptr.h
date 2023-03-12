#pragma once

//Розумні вказівники
//  auto_ptr      (автоматичний вказівник ) С98(добавлений в С++) в С++11(застарілий ) в С++17 (видалений)
//  unique_ptr    ( унікальний вказівник  )
//  shared_ptr    ( розділяючий вказівник )
//  weak_ptr      (   слабкий вказівник   )
//std::auto_ptr<int> ptr;
namespace pointers
{

#define _NODISCARD [[nodiscard]] //Атрибут
	/*Використовується для того щоб якщо метод(функція коли повертає)
	значення його записувала, якщо не запишемо буде попередження*/


template<class T>
struct auto_ptr_ref
	{
		T* reference;
		
		explicit auto_ptr_ref(T* other)      // В нас явний конструктор приймається адреса і присвоюється 
			:reference(other) { }
	};

template<class T>
class auto_ptr
{
private:
#pragma region Поля
		
	T* pointer;
#pragma endregion
public:
#pragma region Псевдоніми 
		using element_type = T;
		/*Створений для того щоб якщо вам заманеться  подивитись тип на який вказує
		вказівник то ми зможемо витянути тип шаблону класу	*/
#pragma endregion

#pragma region Конструктори

		explicit auto_ptr(T* ptr = nullptr) noexcept
			:pointer(ptr)
		{  }

		auto_ptr(auto_ptr& other) noexcept
			:pointer(other.release())
		{  }

		auto_ptr(auto_ptr_ref<T> other) noexcept
		{
			T* ptr = other.reference;
			other.reference = nullptr;
			pointer = ptr;
		}

		template<class _Other>
		auto_ptr(auto_ptr <_Other>& other) noexcept
			: pointer(other.release())
		{  }



#pragma endregion

#pragma region Деструктор
		~auto_ptr()noexcept
		{
			delete pointer;
		}
#pragma endregion

#pragma region Методи 

		_NODISCARD T* get()const noexcept
		{
			return pointer;
		}


		/*Метод relase() занулює вказівник і повертає адресу даних на які він вказує
		Для того щоб інший вказівник з ними працював */
		T* release() noexcept
		{
			T* temp = pointer;
			pointer = nullptr;
			return temp;
		}


		/*
		Метод reset(T* ptr = nullptr)
		Метод приймає адресу даних, які ми встановимо  вказівнику за замовчуванням це(nullptr)
		Якщо ми передали адресу і адреса не дорівнює тій на яку вказував pointer
		то ми видалимо дані на які вказував вказівник раніше і для вказівника встановимо нові дані

		Якщо для reset() ми нічого не передамо то він видалить */

		void reset(T* ptr = nullptr)
		{
			if (ptr != pointer)
				delete pointer;
		}

#pragma endregion

#pragma region Перезавантажені оператори


#pragma region Оператори приведення типу
		/*   Оператор  приведення в інший тип
			 Тобто в нас цей оператор створює вказівник на тип OtherT і передає наш об'єкт  по посиланню і повертається
			  auto_ptr<int>    Ptr  {new int };
			  auto_ptr<double> Ptr2 {   Ptr  };
			  З однаковими типами це робити не можна
		 */
		template<class OtherT>
		operator auto_ptr<OtherT>() noexcept
		{
			return auto_ptr<OtherT>(*this)
		}

		template<class OtherT>
		operator auto_ptr_ref<OtherT>() noexcept
		{
			OtherT* temp = pointer;
			auto_ptr_ref<OtherT> result(temp);
			pointer = nullptr;
			return result;
		}
#pragma endregion

#pragma region Перезавантаження operator =
		template<class OtherT>
		auto_ptr& operator=(auto_ptr<OtherT>& other) noexcept
		{
			reset(other.release())
				return *this;
		}

		auto_ptr& operator=(auto_ptr& other) noexcept
		{
			reset(other.release())
				return *this;
		}

		auto_ptr& operator=(auto_ptr_ref<T>& other) noexcept
		{
			T* ptr = other.reference;
			other.reference = nullptr;
			reset(ptr)
				return *this;
		}
#pragma endregion

#pragma region Перезавантаження operator*() 

		_NODISCARD T& operator*() const noexcept
		{
			return *get();
		}
#pragma endregion

#pragma region Перезавантаження operator->()

		_NODISCARD T& operator->() const noexcept
		{
			return get();
		}
#pragma endregion

#pragma endregion
};

//Спеціалізація для шаблону типу void
	template< >
	class auto_ptr<void>
	{
	public:
		using element_type = void;
	};
}
