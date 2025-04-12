#ifndef ucg_contrast_hpp
#define ucg_contrast_hpp

#include <stdexcept>


// -- U C G  N A M E S P A C E ------------------------------------------------

namespace ucg {


	// -- C O N T R A S T  R A N G E ------------------------------------------

	class contrast_range final {


		private:

			// -- private members ---------------------------------------------

			/* min */
			double _min;

			/* max */
			double _max;


		public:

			// -- public lifecycle --------------------------------------------

			/* percent constructor */
			contrast_range(const double percent)
			: _min{0.0}, _max{0.0} {

				if (percent < 0.0 || percent > 100.0) {
					throw std::out_of_range("percent must be between 0 and 100");
				}

				// calculate the min and max values [0.0, 100.0]
				_min = 0.5 - (percent / 200.0);
				_max = 0.5 + (percent / 200.0);
			}


			// -- public accessors --------------------------------------------

			/* min */
			auto min(void) const noexcept -> double {
				return _min;
			}

			/* max */
			auto max(void) const noexcept -> double {
				return _max;
			}

	}; // class contrast

} // namespace ucg

#endif // ucg_contrast_hpp
