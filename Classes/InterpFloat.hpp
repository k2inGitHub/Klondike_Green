//
//  InterpFloat.hpp
//  Klondike
//
//  Created by 宋扬 on 16/9/30.
//
//

#ifndef InterpFloat_hpp
#define InterpFloat_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class InterpFloat {

public:
    enum EInterpType{
        INTERP_TYPE_LINEAR,
        //INTERP_TYPE_EASE_TO,
        //INTERP_TYPE_EASE_FROM,
        //INTERP_TYPE_EASE_BETWEEN
    };
    
    InterpFloat ()
    {
        m_current = 0;
        m_interpType = INTERP_TYPE_LINEAR;
    };
    InterpFloat (float value)
    {
        m_current = value;
        m_interpType = INTERP_TYPE_LINEAR;
    };
    InterpFloat (float value, EInterpType interpType)
    {
        m_current = value;
        m_interpType = interpType;
    };
    
    void InterpTo (float end, float time)
    {
        this->InterpTo (end, time, this->m_interpType);
    };
    
    void InterpTo (float end, float time, EInterpType interpType)
    {
        this->m_start = this->m_current;
        this->m_end = end;
        this->m_timer = 0.f;
        this->m_time = time;
        this->m_interpType = interpType;
        if (time <= 0.f)
        {
            this->m_current = end;
        }
    };
    
     float Get ()
    {
        return this->m_current;
    };
     float GetEnd ()
    {
        return this->m_end;
    };
     float GetStart ()
    {
        return this->m_start;
    };
    
    // 是否结束
     bool IsDone ()
    {
        return this->m_current == this->m_end;
    };
    
     void Set (float value)
    {
        this->InterpTo (value, 0.f, this->m_interpType);
    };
    
    
     void Update (float dt)
    {
        if (this->m_timer < this->m_time)
        {
            this->m_timer += dt;
            if (this->m_timer > this->m_time)
            {
                this->m_timer = this->m_time;
            }
            
            float smoothedTime = this->GetSmoothedTime01 ();
            this->m_current = this->m_start + (this->m_end - this->m_start) * smoothedTime;
        }
    };
    
     float GetRawTime01 ()
    {
        return this->m_timer / this->m_time;
    };
    
     float GetSmoothedTime01 ()
    {
        return EaseAsType (m_interpType, this->m_timer / this->m_time);
    };
    
    static float EaseAsType (EInterpType type, float a)
    {
        switch (type)
        {
            case INTERP_TYPE_LINEAR:
                return clampf(a, 0, 1);
        }
    };
    
private:
    float m_start;
    float m_end;
    float m_current;
    float m_time;
    float m_timer;
    EInterpType m_interpType;
    
};


#endif /* InterpFloat_hpp */
