#ifndef stdbool_h_
#define stdbool_h_

/**
******************************************************************************
                            Not a bit but a byte
******************************************************************************
**/
typedef unsigned char bool;
enum STAND_BOOL_ {
    false = (bool)(0U),
    true  = (bool)(1U)
};

#endif
