// M6-5-5.cpp

bool test_a ( bool * pB )
{
  *pB = (1!=1) ? true : false;
  return *pB;
}

bool test (int v)
{
  return (v > 10) ? true : false;
}

void f()
{
  int x;
  bool bool_a = false;
  for ( x = 0; ( x < 10 ) && !bool_a; ++x )
  {
    if ( true )
    {
      bool_a = true; // UndCC_Valid
    }
  }

  for ( x = 0;
  ( x < 10 ) && test_a ( &bool_a ); // UndCC_Violation
  ++x ) 
  { 
    // ... 
  }
  
  volatile bool status;
  for ( x = 0; ( x < 10 ) && status; ++x) // UndCC_Valid
  { }
  
  for ( x = 0; x < 10; bool_a = test( ++x ) ) // UndCC_Violation
  { }
  
  int y = 0;
  for ( x = 0; x < 10; x = y++ ) // UndCC_Violation
  { 

  }
  
  for(int x =0, y=10; x>y; x++, y--) // UndCC_Valid
  {

  }

}

// Issue #4880: false positives reported by wmullen@dekaresearch.com

class Parameter {
public:
  void clear() {}
};

class Container {
  unsigned m_count;
  Parameter* m_params[10];
public:
  unsigned numParameters() { return m_count; }
  Parameter* operator[](unsigned i) { return m_params[i]; }
  void addParameter(Parameter* p) { m_params[m_count++] = p; }
  void parameters_clear()
  {
    // Function call in condition whose return-entity is modified elsewhere
    // must not be flagged as a loop-control-variable modification.
    for (unsigned i = 0u; i < numParameters(); i++) // UndCC_Valid
    {
      m_params[i]->clear();
    }
  }
};

struct FileEntry { const char* name; };
struct FileList {
  FileEntry* begin() { return nullptr; }
  FileEntry* end()   { return nullptr; }
};

struct JsonObject { JsonObject() {} };

void range_based_for_is_not_a_classic_for(FileList& files)
{
  // Range-based for loops have no init/condition/expression structure,
  // so this rule does not apply.
  int counter = 0;
  for (const FileEntry& entry : files) // UndCC_Valid
  {
    JsonObject obj;
    counter = 1;
    (void)obj;
    (void)entry;
  }
}
