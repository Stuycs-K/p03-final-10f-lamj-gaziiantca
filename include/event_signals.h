#ifndef EVENT_SIGNALS_H
  #define EVENT_SIGNALS_H

  #include "linked_list.h"

  typedef void (*SignalCallback)(void* context, void* args);

  typedef struct Signal {
    LinkedList* connections;
  } Signal;

  typedef enum ConnectionMode {
    NO_MODE, 
    LISTEN_ONCE
  } ConnectionMode;

  typedef struct Connection {
    Signal* parent;
    Node* connectionNode;

    SignalCallback callback;
    ConnectionMode mode;
    void* context;
  } Connection;

  Signal* Signal_new();
  void Signal_Fire(Signal* self, void* arg);
  Connection* Signal_Connect(Signal* self, SignalCallback callback, void* context);
  Connection* Signal_ConnectOnce(Signal* self, SignalCallback callback, void* context);
  void Signal_Free(Signal* self);

  void Connection_Disconnect(Connection* self);

#endif
