#define JSON_THROW(exception) throw exception

enum Json
{
  null,
  object,
  array,
  string,
  boolean,
  number_integer,
  number_unsigned,
  number_float,
  binary,
  discarded,
};

void f(Json type)
{
  switch (type) {
    case number_unsigned:
    case number_integer:
    case number_float:
      break;

    case null:
    case object:
    case array:
    case string:
    case boolean:
    case binary:
    case discarded:
    default:
      JSON_THROW("Expected a number");
  }
}
