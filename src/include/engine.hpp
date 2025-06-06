#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <array>

// Core/Utility headers
#include <clockObject.h>
#include <trueClock.h>
#include <eventQueue.h>
#include <eventHandler.h>
// Graphics System headers
#include <windowProperties.h>
#include <frameBufferProperties.h>
#include <graphicsPipe.h>
#include <graphicsPipeSelection.h>
#include <graphicsEngine.h>
// camera and lenses
#include <camera.h>
#include <orthographicLens.h>
// Input and Event-Handling headers
#include <mouseWatcher.h>
#include <mouseAndKeyboard.h>
#include <keyboardButton.h>
#include <buttonThrower.h>
// Scene Graph and Rendering headers
#include <nodePath.h>
#include <pgTop.h>
#include <dataGraphTraverser.h>
#include <dataNodeTransmit.h>
// Asynchronous Task Management
#include <asyncTaskManager.h>
#include <genericAsyncTask.h>
// other
#include "sceneCam.hpp"
#include "axisGrid.hpp"
#include "resourceManager.hpp"
#include "mouse.hpp"

class Engine {
struct EventInfo {
    char owner[16];
    char event_id[16];
    std::function<void()> callback;

    // Constructor to initialize event
    EventInfo(const char* owner_name, const char* event_id, std::function<void()> cb)
        : callback(std::move(cb)) {
        std::memset(this->owner, 0, sizeof(this->owner));
        strncpy_s(this->owner, owner_name, sizeof(this->owner) - 1);

        std::memset(this->event_id, 0, sizeof(this->event_id));
        strncpy_s(this->event_id, event_id, sizeof(this->event_id) - 1);
    }
};

struct EventListenerInfo {
    char listener[16];
    std::function<void(std::string)> callback;

    // Constructor to initialize event
    EventListenerInfo(const char* listener, std::function<void(std::string)> cb)
        : callback(std::move(cb)) {
        std::memset(this->listener, 0, sizeof(this->listener));
        strncpy_s(this->listener, sizeof(this->listener), listener, sizeof(this->listener) - 1);
    }
};

public:
    Engine();
    ~Engine();

    // fields
	PT(GraphicsPipe)      pipe;
    PT(GraphicsEngine)    engine;
    PT(GraphicsWindow)    win;
    PT(DisplayRegion)     dr;
    PT(DisplayRegion)     dr2D;

    PT(MouseWatcher)      mouse_watcher;
	
    std::vector<NodePath> mouse_watchers;
	std::vector<NodePath> button_throwers;

    NodePath              data_root;
    DataGraphTraverser    data_graph_trav;
    EventQueue*           event_queue;
    EventHandler*         event_handler;

    NodePath              render;
    SceneCam              scene_cam;
    NodePath              render2D;
    NodePath              aspect2D;
        
    NodePath              pixel2D;
    NodePath              cam2D;

    Mouse                 mouse;
    ResourceManager       resource_manager;
    AxisGrid              axis_grid;
	
    std::vector<EventInfo> events_map;
    std::vector<EventListenerInfo> events_listeners;

    bool should_repaint;
	
    // methods
    void update();
    void on_evt_size();
    
    void accept(const std::string&, std::function<void()>);
    void accept(const std::string&, const std::string&, std::function<void()>);
    void Engine::ignore(const std::string&);
    void Engine::ignore(const std::string&, const std::string&);
    void Engine::ignore_all();
    bool has_event(const std::string&, const std::string&);
	bool has_event(const std::string&);
    
    void add_event_listener(const std::string&, std::function<void(std::string)>);
    void remove_event_listener(const std::string&);
    void clear_event_listeners();
 
	void dispatch_event(const char*);
	void dispatch_events(bool ignore_mouse = false);
    
	void trigger(const char*);
    void show_axis_grid(bool show = false);
	void clean_up();
    
	float get_aspect_ratio();
    LVecBase2i get_size();
	
private:
    void create_win();
    void create_3d_render();
    void create_2d_render();
    void create_default_scene();
    void create_axis_grid();
    void setup_mouse_keyboard(PT(MouseWatcher)& mw);
    void reset_clock();
	void process_events(CPT_Event event);
		
	// cache
	std::vector<std::pair<CPT_Event, std::vector<void*>>> panda_events;
    LVecBase2i window_size;
    float aspect_ratio;
};

#endif