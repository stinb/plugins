// AUTOSAR

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class StackBitmap
{
public:
  constexpr static size_t maxSize = 65535;
  using BitmapRawType = std::array<uint8_t, maxSize>;
  StackBitmap(const std::string& path, uint32_t bitmapSize)
  {
    // read bitmapSize bytes from the file path
  }

  const BitmapRawType& GetBitmap() const noexcept { return bmp; }

private:
  BitmapRawType bmp;
};

void AddWidgetToLayout(int32_t row, int32_t col)
{
  auto idx = std::make_pair(row, col); // UndCC_Valid
  auto spIdx = std::make_shared<std::pair<int32_t, int32_t>>( // UndCC_Violation
  row, col);
  // addWidget to index idx
}

void CalcAverageBitmapColor(const std::string& path, uint32_t bitmapSize)
{
  std::vector<uint8_t> bmp1(bitmapSize); // UndCC_Valid
  // read bitmap from path
  StackBitmap bmp2(path, bitmapSize); // UndCC_Violation
  bmp2.GetBitmap();
}

int main(int, char**)
{
  AddWidgetToLayout(5, 8);
  CalcAverageBitmapColor("path/to/bitmap.bmp", 32000);
}
