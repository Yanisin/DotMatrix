#ifndef HW_DEFS_H
#define HW_DEFS_H


#if defined (HW_38x38_rev_0_1)

#include "hw_defs_38x38_rev_0_1.h"

#elif defined (HW_38x38_rev_0_3)

#include "hw_defs_38x38_rev_0_3.h"

#elif defined (HW_60x60_rev_0_2)

#include "hw_defs_60x60_rev_0_2.h"

#elif defined (HW_60x60_rev_0_4)

#include "hw_defs_60x60_rev_0_4.h"


#elif defined (HW_60x60_rev_0_5)

#include "hw_defs_60x60_rev_0_5.h"

#else

#error HW version not defined

#endif /*HW_VERSION*/

#endif
