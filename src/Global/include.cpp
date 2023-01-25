#include <stdint.h>
#include <stdbool.h>

#include "driverlib/debug.h"

#include "Global/Include.h"
#include "INC/debug.h"

static bool strcompare (const char * des, const char *src) {
    uint32_t index = 0;

    if(des == 0 || src == 0) {
        SystemDebug.log(DEBUG_ERR, "Input argument pointer with value NULL!!");
        return false;
    }

    while (1) {
        if((des[index] == '\0') || (src[index] == '\0')) {
            if(des[index] == src[index])
                return true;
            else
                return false;
        }
        if(des[index] != src[index]) {
            return false;
        }
        index++;
    }
}

// struct Answer;
// typedef Answer (*impl_fn_type)( long, long );

// struct FnPlusArgs
// {
    // impl_fn_type fn_;
    // long arg1_;
    // long arg2_;

    // FnPlusArgs(
        // impl_fn_type fn,
        // long arg1,
        // long arg2
    // );

    // Answer operator()();
// };

// impl_fn_type null_fn = NULL;
// FnPlusArgs null_fn_plus_args( null_fn, 0, 0 );

// struct Answer
// {
    // bool finished_;
    // FnPlusArgs tail_call_;
    // long value_;

    // Answer( bool finished, FnPlusArgs tail_call, long value );
// };


// FnPlusArgs::FnPlusArgs(
    // impl_fn_type fn,
    // long arg1,
    // long arg2
// )
// : fn_( fn )
// , arg1_( arg1 )
// , arg2_( arg2 )
// {
// }

// Answer FnPlusArgs::operator()()
// {
    // return fn_( arg1_, arg2_ );
// }


// Answer::Answer( bool finished, FnPlusArgs tail_call, long value )
// : finished_( finished )
// , tail_call_( tail_call )
// , value_( value )
// {
// }

// Answer times_two_tail_call_impl( long acc, long i )
// {
    // if ( i == 0 )
    // {
        // return Answer( true, null_fn_plus_args, acc );
    // }
    // else
    // {
        // return Answer(
            // false,
            // FnPlusArgs(
                // times_two_tail_call_impl,
                // acc + 2,
                // i - 1
            // ),
            // 0
        // );
    // }
// }

// long trampoline( Answer answer )
// {
    // while ( !answer.finished_ )
    // {
        // answer = answer.tail_call_();
    // }
    // return answer.value_;
// }

// long times_two_tail_call( long n )
// {
    // return trampoline( Answer(
        // false,
        // FnPlusArgs( times_two_tail_call_impl, 0, n ),
        // 0 ) );
// }


// int main(int argc, char ** argv)
// {
    // printf("%d",times_two_tail_call(10000000));
    
    // return 0;
// }

#if defined(ccs)
uint32_t
_getStackPointer(void)
{
    //
    // Read BASEPRI
    //
    __asm("    mov     r0, sp\n"
          "    bx      lr\n");

    //
    // The following keeps the compiler happy, because it wants to see a
    // return value from this function.  It will generate code to return
    // a zero.  However, the real return is the "bx lr" above, so the
    // return(0) is never executed and the function returns with the value
    // you expect in R0.
    //
    return(0);
}
#endif

#if defined(codered) || defined(gcc) || defined(sourcerygxx)
uint32_t __attribute__((naked))
_getStackPointer(void)
{
    uint32_t ui32Ret;

    //
    // Read BASEPRI
    //
    __asm("    mov     r0, sp\n"
          "    bx      lr\n"
          : "=r" (ui32Ret));

    //
    // The return is handled in the inline assembly, but the compiler will
    // still complain if there is not an explicit return here (despite the fact
    // that this does not result in any code being produced because of the
    // naked attribute).
    //
    return(ui32Ret);
}
#endif

static uint32_t _recursiveGetIndex(uint32_t startIndex, void *vlistName, const char * targetStr, uint32_t offset) {
    // C++ doesn't support tail-call optimized (some said), there is trampoline techique but haven't yet implemented
    stringSearch *listName;
#ifdef DEBUG
recursive_loop:
#endif
    //printf("[TAIL-CALL] 0x%08x \r\n", _getStackPointer());
    listName = (stringSearch *)(vlistName + offset*startIndex);
    if((listName == 0) || (targetStr == 0) || (listName->name == 0) || strcompare(listName->name, "NULL")) {
        return 0xffffffff;
    }

    if(strcompare(listName->name, targetStr)) {
        return startIndex;
    }
    else {

#ifdef DEBUG
        startIndex++;
        goto recursive_loop;
#else
        // C++ doesn't support tail-call optimized (some said)
        return _recursiveGetIndex(++startIndex, vlistName, targetStr, offset);
#endif
    }
}

uint32_t getIndexFromStrList(void *listName, const char *targetStr, uint32_t offset) {
    return _recursiveGetIndex(0, listName, targetStr, offset);
}

PHY_Reg_Bit::PHY_Reg_Bit(PHY_Register *parent, const char* bitName, uint8_t lowBit, uint8_t highBit) {
    if ((lowBit > 31) && (highBit > 31)) {
        SystemDebug.log(DEBUG_ERR, "lowBit or highBit is bigger than 31");
        return;
    }

    this->name   = bitName;
    this->parent = parent;

    if (lowBit > highBit) {
        this->bitmask = bit_mask(highBit, lowBit);
        this->pos = highBit;
    }
    else {
        this->bitmask = bit_mask(highBit, lowBit);
        this->pos = lowBit;
    }
}

PHY_Reg_Bit::PHY_Reg_Bit(void) {
    // do no thing, just to create an object
}

void PHY_Reg_Bit::update(PHY_Register *parent, const char *bitName, uint8_t lowBit, uint8_t highBit) {
    if ((lowBit > 31) && (highBit > 31)) {
        SystemDebug.log(DEBUG_ERR, "lowBit or highBit is bigger than 31");
        return;
    }

    this->name   = bitName;
    this->parent = parent;

    if (lowBit > highBit) {
        this->bitmask = bit_mask(highBit, lowBit);
        this->pos = highBit;
    }
    else {
        this->bitmask = bit_mask(highBit, lowBit);
        this->pos = lowBit;
    }
}

// read bits
uint32_t PHY_Reg_Bit::operator() (void) {
    return (((*this->parent)() & bitmask) >> this->pos);
}

// write bits
void PHY_Reg_Bit::operator= (uint32_t value) {
    (*this->parent) = ((*this->parent)() & (~this->bitmask)) | ((value & (this->bitmask >> this->pos)) << this->pos);
}

void PHY_Reg_Bit::dumpInfo(void) {
    printf("PHY_Reg_Bit info:\r\n");
    printf("Name: %s\r\n", this->name);
    printf("Parent's name: %s\r\n", this->parent->name);
    printf("Pos: %d \r\n", this->pos);
    printf("Bitmask: %08x\r\n", this->bitmask);
    printf("Bit value: %d\r\n", (*this->parent)[this->name]());
}

PHY_Register::PHY_Register(const char *name, uint32_t baseAddr, uint32_t offset, PHY_Reg_Bit *bitList, regBitInfo *bitInfoList) {
    uint32_t index = 0;

    this->name = name;
    this->baseAddr = baseAddr;
    this->addrOffset = offset;
    this->regAddress = (uint32_t *)(this->baseAddr + this->addrOffset);
    this->bitList = bitList;
    this->bitInfoList = bitInfoList;

    for (index = 0; !strcompare("NULL", bitInfoList[index].name) ; index++) {
        this->bitList[index].update(this, bitInfoList[index].name, bitInfoList[index].lowBit, bitInfoList[index].highBit);
    }
}


PHY_Register::PHY_Register(void) {
    // do no thing, just create object
}

void PHY_Register::update(const char *name, uint32_t baseAddr, uint32_t offset, PHY_Reg_Bit *bitList, regBitInfo *bitInfoList) {
    uint32_t index = 0;

    this->name = name;
    this->baseAddr = baseAddr;
    this->addrOffset = offset;
    this->regAddress = (uint32_t *)(this->baseAddr + this->addrOffset);
    this->bitList = bitList;
    this->bitInfoList = bitInfoList;

    for (index = 0; !strcompare("NULL", bitInfoList[index].name) ; index++) {
        this->bitList[index].update(this, bitInfoList[index].name, bitInfoList[index].lowBit, bitInfoList[index].highBit);
    }
}

void PHY_Register::baseAddrUpdate(uint32_t baseAddr) {
    this->baseAddr = baseAddr;
    this->regAddress = (uint32_t *)(this->baseAddr + this->addrOffset);
}

PHY_Reg_Bit PHY_Register::operator[](const char *bitName) {
    uint32_t index = 0xffffffff;
    index = getIndexFromStrList(this->bitList, bitName, sizeof(PHY_Reg_Bit));
    if(0xffffffff == index) {
        SystemDebug.log(DEBUG_ERR, "Register [%s] doesn't have [%s] bit!!!", this->name, bitName);
        ASSERT(0);
    }
    return this->bitList[index];
}

uint32_t PHY_Register::operator()(void) {
    return *(uint32_t*) this->regAddress;
}

void PHY_Register::operator=(int32_t value) {
    *(uint32_t*) this->regAddress = value;
}
