char n_array[] = "Hello" "World"; // UndCC_Valid
wchar_t w_array[] = L"Hello" L"World"; // UndCC_Valid
wchar_t mixed[] = "Hello" L"World"; // UndCC_Violation

// MISRA test
const char *s0 = "Hello" "World"; // UndCC_Valid
const wchar_t *s1 = L"Hello" L"World"; // UndCC_Valid
const wchar_t *s2 = "Hello" L"World"; // UndCC_Violation
// const wchar_t *s3 = u"Hello" L"World"; // Strict parser error
// const wchar_t *s3a = u8"Hello" L"World"; // Strict parser error
const char *s4 = u8R"#(Hello)#" u8"World"; // UndCC_Valid
const char *s5 = u8R"#(Hello)#" "World"; // UndCC_Violation