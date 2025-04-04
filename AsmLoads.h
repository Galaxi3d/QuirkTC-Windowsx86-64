#ifndef ASMLOADS_H
#define ASMLOADS_H
#include <string>
#include "Assembly.h"

class AsmLoads
{
private:
    std::string Address;
    size_t Index;

    inline bool IsUpcomingCondition() const;
        
    inline bool IsUpcomingMath() const;		

    inline bool IsUpcomingSetToStack() const;

    inline bool IsUpcomingSetToParameter() const;

    inline bool IsUpcomingModifyVariable() const;

    inline uint8_t GenericLoad();

    inline void BinaryOperation() const;

    inline void Condtional() const;

	inline void SetToStack() const;

	inline void SetToParameter() const;

    inline void ModifyVariable() const;

    inline static std::string GetStackAddress(size_t Index);

    inline static std::string GetConstAddress(size_t Index);    
public:

	AsmLoads(size_t index) : Index(index) {}

    /// <summary>
    /// 
    /// </summary>
    /// <returns>true when 3 instructions are compiled. false when 1 is</returns>
    uint8_t LoadFromStack();
    
    /// <summary>
    /// 
    /// </summary>
    /// <returns>true when 3 instructions are compiled. false when 1 is</returns>
    uint8_t LoadConst();
};
#endif