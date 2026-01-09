#include "event_signals.h"

void Signal_Fire(Signal* self, void* arg) {
  LinkedList* connections = self->connections;
  Node* current = connections->front;

  while (current) {
    Connection* currentConn = current->entry;
    currentConn->callback(arg);
    if (currentConn->mode == LISTEN_ONCE) {
      LinkedList_deleteNode(self, currentConn->connectionNode);
    }
    current = current->next
  }
}

Connection* Signal_Connect(Signal* self, SignalCallback callback) {
  Connection* newConn = (Connection*) malloc(sizeof(Connection));
  newConn->parent = self;
  newConn->connectionNode = LinkedList_insertEnd(self->connections, newConn);
  newConn->callback = callback;
  newConn->mode = NO_MODE;

  return newConn;
}

Connection* Signal_ConnectOnce(Signal* self, SignalCallback callback) {
  Connection* newConn = Signal_Connect(self, callback);
  newConn->mode = LISTEN_ONCE;

  return newConn;
}

void Connection_Disconnect(Connection* self) {
  LinkedList_deleteNode(self->parent->connections, self->connectionNode);
}
