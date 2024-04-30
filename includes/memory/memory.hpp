/*****************************************************************************/
/*                                                                           */
/*                 ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░                   */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░                         */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒▒▓███▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                  */
/*                  ░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░                   */
/*                                                                           */
/*****************************************************************************/

#pragma once

#ifndef UCG_MEMORY_HEADER
#define UCG_MEMORY_HEADER

#include <sys/mman.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>


// -- U C G  N A M E S P A C E ------------------------------------------------

namespace ucg {


	// -- M E M O R Y ---------------------------------------------------------

	template <decltype(sizeof(0)) __mega>
	class memory final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = ucg::memory<__mega>;

			/* size type */
			using size_type = decltype(sizeof(0));


			// -- public lifecycle --------------------------------------------

			/* deleted copy constructor */
			memory(const self&) = delete;

			/* deleted move constructor */
			memory(self&&) = delete;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public static methods ---------------------------------------

			/* allocate */
			template <typename __type>
			static auto allocate(const size_type size) -> __type* {
				return static_cast<__type*>(self::shared().allocate(size * sizeof(__type)));
			}


		private:

			// -- private types -----------------------------------------------

			/* max integer type */
			#if defined(__SIZEOF_INT128__)
			using max_type = __int128;
			#else
			using max_type = void*;
			#endif


			// -- private constants -------------------------------------------

			/* bytes */
			static constexpr auto _bytes = __mega * 1024 * 1024;

			/* mmap protection */
			static constexpr auto _protection = PROT_READ | PROT_WRITE;

			/* mmap flags */
			static constexpr auto _flags = MAP_PRIVATE | MAP_ANONYMOUS;

			/* alignment */
			static constexpr auto _alignment = sizeof(max_type);



			/* max */
			static consteval auto max(void) -> size_type {
				// number of bits in type
				constexpr size_type bits = static_cast<size_type>(sizeof(size_type) * 8U);

				size_type count = 0U;

				// loop over bits
				for (size_type i = 0U; i < bits; ++i)
					// add shifted bit to max
					count |= static_cast<size_type>(static_cast<size_type>(1) << i);

				return count;
			}

			/* align */
			static auto align(const size_type size) -> size_type {
				// alignment - 1
				constexpr auto value = _alignment - 1U;
				// return aligned size
				return (size + value) & ~value;
			}




			// -- private lifecycle -------------------------------------------

			/* default constructor */
			memory(void)
			: _data{nullptr}, _capacity{0U}, _size{0U} {

				// get page size
				const auto page = ::sysconf(_SC_PAGESIZE);

				if (page < 0)
					throw std::runtime_error{"failed to get page size"};

				// calculate capacity in pages
				_capacity = _bytes / static_cast<size_type>(page);

				// allocate initial memory (suppose to be aligned)
				_data = ::mmap(nullptr, _capacity * static_cast<size_type>(page),
										_protection, _flags, -1, 0);

				if (_data == MAP_FAILED)
					throw std::runtime_error{"failed to allocate initial memory"};

				_capacity = _bytes;

			}

			/* destructor */
			~memory(void) noexcept {
				// deallocate memory
				::munmap(_data, _capacity);
			}


			// -- private methods ---------------------------------------------

			/* allocate */
			auto allocate(const size_type size) -> void* {
				// check if size is valid
				if (size == 0)
					throw std::runtime_error{"invalid size"};

				// check overflow
				// if (248 > (255 -  8))       -> if (248 > 247) -> true
				// if (248 > (255 - (8 - 1U))) -> if (248 > 248) -> false

				if (size > (self::max() - (_alignment - 1U)))
					throw std::runtime_error{"size exceeds maximum capacity"};

				// align size
				const auto aligned = self::align(size);

				// check if size is greater than remaining capacity
				if (_size + aligned > _capacity)
					throw std::runtime_error{"size exceeds remaining capacity"};

				// allocate memory
				auto* ptr = static_cast<char*>(_data) + _size;
				_size += aligned;

				return ptr;
			}


			// -- private static methods --------------------------------------

			/* shared */
			static auto shared(void) -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* data */
			void* _data;

			/* capacity */
			size_type _capacity;

			/* size */
			size_type _size;

	}; // class memory


	// -- type aliases --------------------------------------------------------

	/* memory 1 mega */
	using allocator = ucg::memory<1>;

} // namespace ucg

#endif // UCG_MEMORY_HEADER
