#include <cstdint>
class C
{
    public:
        int32_t x;
        C ( )
            try
            {
                // Action that may raise an exception
            }
        catch ( ... )
        {
            if ( 0 == x ) // UNDCC_Violation – x may not exist at this point
            {
                // Action dependent on value of x
            }
        }
        ~C ( )
            try
            {
                // Action that may raise an exception
            }
        catch ( ... )
        {
            if ( 0 == x ) // UNDCC_Violation – x may not exist at this point
            {
                // Action dependent on value of x
            }
        }
};
