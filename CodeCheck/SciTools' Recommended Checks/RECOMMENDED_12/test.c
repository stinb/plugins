enum light { red, amber, red_amber, green };
enum light next_light ( enum light c )
{
  enum light res;
  switch ( c )
  {
    case red:
      res = red_amber;
      break;
    case red_amber:
      res = green;
      break;
    case green:
      res = amber;
      break;
    case amber:
      res = red;
      break;
    default:
      {
        /*
         * This default will only be reachable if the parameter c
         * holds a value that is not a member of enum light.
         */
        error_handler ( );
        break;
      }
  }
  return res;
  res = c; /* UNDCC_Violation - this statement is
              * certainly unreachable */
}
