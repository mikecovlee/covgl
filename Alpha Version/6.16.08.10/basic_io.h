#include "cgk3.h"
namespace cov {
	namespace gl {
		class mouse {
		public:
			enum class events {
			    null,left_click,right_click,roller_click,roller_up,roller_down
			};
		protected:
			events mEvent=events::null;
			baseCtrl ** mCMap;
			std::size_t mWidth, mHeight, mCX, mCY;
		public:
			mouse():mCMap(nullptr), mWidth(0), mHeight(0), mCX(0), mCY(0)
			{
			}
			mouse(std::size_t w, std::size_t h):mCMap(new baseCtrl *[w * h]), mWidth(w), mHeight(h), mCX(0), mCY(0)
			{
			}
			mouse(const mouse &) = delete;
			mouse(mouse &&) = delete;
			~mouse()
			{
				delete[]mCMap;
			}
			mouse & operator=(const mouse &) = delete;
			mouse & operator=(mouse &&) = delete;
			events mouse_event() const
			{
				return mEvent;
			}
			void resize(std::size_t w, std::size_t h)
			{
				delete[]mCMap;
				mCMap = new baseCtrl *[w * h];
				mWidth = w;
				mHeight = h;
			}
			void clear()
			{
				if (mCMap != nullptr) {
					delete[]mCMap;
					mCMap = new baseCtrl *[mWidth * mHeight];
				}
			}
			void login(const std::array < std::size_t, 2 > &posit, const std::array < std::size_t, 2 > &size, baseCtrl * ptr)
			{
				std::size_t col(posit[0]), row(posit[1]), sw(size[0]), sh(size[1]);
				if (mCMap == nullptr)
					throw std::logic_error(__func__);
				if (col < 0 || row < 0 || col > mWidth || row > mHeight)
					throw std::out_of_range(__func__);
				for (std::size_t r = row; r < mHeight && r - row < sh; ++r)
					for (std::size_t c = col; c < mWidth && c - col < sw; ++c)
						mCMap[r * mWidth + c] = ptr;
			}
			void login_with_image(const std::array < std::size_t, 2 > &posit, const image & img, baseCtrl * ptr)
			{
				login(posit, {img.width(), img.height()}, ptr);
			}
			void logout(baseCtrl * ptr)
			{
				for (std::size_t i = 0; i < mWidth * mHeight; ++i)
					if (mCMap[i] == ptr)
						mCMap = nullptr;
			}
			void active(const std::array < std::size_t, 2 > &posit,events ev)
			{
				if(ev==events::null)
					throw std::invalid_argument(__func__);
				if (mCMap == nullptr)
					throw std::logic_error(__func__);
				if (posit[0] < 0 || posit[1] < 0 || posit[0] > mWidth || posit[1] > mHeight)
					throw std::out_of_range(__func__);
				mEvent=ev;
				baseCtrl *obj = mCMap[posit[1] * mWidth + posit[0]];
				obj->mouse_event.active(obj);
			}
		};
	}
}