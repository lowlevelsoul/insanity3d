//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include <UIKit/UIKit.h>
#include "i3d/input/Input_ios.h"
#include <mutex>


static uint8_t inputSystemMem[ sizeof( i3d::InputSystemIos ) ];
i3d::InputSystemIos * inputLocal = nullptr;
i3d::InputSystem * input = nullptr;

namespace i3d {

    //======================================================================================================================
    void InputCreate() {        
        if (inputLocal == nullptr ) {
            inputLocal = new(inputSystemMem) InputSystemIos;
            input = inputLocal;
        }
    }

    //======================================================================================================================
    void InputDestroy() {
        if ( inputLocal != nullptr ) {
            inputLocal->~InputSystemIos();
            inputLocal = nullptr;
            input = nullptr;
        }
    }
        
    //======================================================================================================================
    InputSystemIos::InputSystemIos() {
        m_events[0].Initialise(EVENT_CAPACITY);
        m_events[1].Initialise(EVENT_CAPACITY);
        m_currEvents = 0;
    }

    //======================================================================================================================
    InputSystemIos::~InputSystemIos() {
        m_events[0].Finalise();
        m_events[1].Finalise();
    }

    //======================================================================================================================
    void InputSystemIos::PushEvent(const Event& ev) {
        m_mutex.lock();
        EventArray& events = m_events[m_currEvents];
            
        if (events.m_eventCount < EVENT_CAPACITY) {
            events.m_events[events.m_eventCount] = ev;
            ++events.m_eventCount;
        }
        
        m_mutex.unlock();
    }

    //======================================================================================================================
    void InputSystemIos::PushKeyEvent(KEY key, char charPressed, uint32_t modifier, float value) {
        Event ev;
        
        ev.m_type = EVENT_TYPE_KEY;
        ev.m_event.m_key.m_key = key;
        ev.m_event.m_key.m_char = charPressed;
        ev.m_event.m_key.m_modifier = modifier;
        ev.m_event.m_key.m_value = value;
        
        PushEvent(ev);
    }

    //======================================================================================================================
    InputSystem::EventArray& InputSystemIos::SwapEvents() {
        m_mutex.lock();
        
        EventArray& prevEvents = m_events[m_currEvents];
        m_currEvents = (m_currEvents + 1) % (uint32_t) EVENT_ARRAY_COUNT;
        m_events[m_currEvents].Reset();
        
        m_mutex.unlock();
        
        return prevEvents;
    }

    //======================================================================================================================
    void InputSystemIos::Think(Event*& eventArray, uint32_t& eventCount) {
        EventArray& events = SwapEvents();
        
        eventArray = &events.m_events[0];
        eventCount = events.m_eventCount;
    }

    //======================================================================================================================
    void InputSystemIos::OnTouchBegin(UITouch * event) {
        CGPoint touchLocation = [event locationInView:nil];
        
        Event ev;
        ev.m_type = EVENT_TYPE_TOUCH;
        ev.m_event.m_touch.m_id = 0;
        ev.m_event.m_touch.m_type = TouchEvent::TYPE_BEGIN;
        ev.m_event.m_touch.m_x = touchLocation.x;
        ev.m_event.m_touch.m_y = touchLocation.y;
        
        PushEvent(ev);
    }

    //======================================================================================================================
    void InputSystemIos::OnTouchEnd(UITouch * event) {
        CGPoint touchLocation = [event locationInView:nil];
        
        Event ev;
        ev.m_type = EVENT_TYPE_TOUCH;
        ev.m_event.m_touch.m_id = 0;
        ev.m_event.m_touch.m_type = TouchEvent::TYPE_END;
        ev.m_event.m_touch.m_x = touchLocation.x;
        ev.m_event.m_touch.m_y = touchLocation.y;
        
        PushEvent(ev);
    }

    //======================================================================================================================
    void InputSystemIos::OnTouchMove(UITouch * event) {
        CGPoint touchLocation = [event locationInView:nil];
        CGPoint touchLocationPrev = [event previousLocationInView:nil];
        
        Event ev;
        ev.m_type = EVENT_TYPE_TOUCH;
        ev.m_event.m_touch.m_id = 0;
        ev.m_event.m_touch.m_type = TouchEvent::TYPE_MOVED;
        ev.m_event.m_touch.m_x = touchLocation.x;
        ev.m_event.m_touch.m_y = touchLocation.y;
        
        PushEvent(ev);
    }
}
