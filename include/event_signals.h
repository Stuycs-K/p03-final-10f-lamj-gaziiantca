#ifndef EVENT_SIGNALS_H
  #define EVENT_SIGNALS_H

  #include "linked_list.h"

  typedef void (*SignalCallback)(void*);

  typedef struct Signal {
    LinkedList* connections;
  } Signal;

  enum ConnectionMode {NO_MODE, LISTEN_ONCE};

  typedef struct Connection {
    Signal* parent;
    Node* connectionNode;

    SignalCallback callback;
    ConnectionMode mode;
  } Connection;

  void Signal_Fire(Signal* self, void* arg);
  Connection* Signal_Connect(Signal* self, SignalCallback callback);
  Connection* Signal_ConnectOnce(Signal* self, SignalCallback callback);

  void Connection_Disconnect(Connection* self);
#endif
