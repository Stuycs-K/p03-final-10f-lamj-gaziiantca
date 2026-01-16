#include "event_signals.h"
#include "linked_list.h"

#include <stdlib.h>

Signal* Signal_new() {
  Signal* newSignal = (Signal*) malloc(sizeof(Signal));
  newSignal->connections = LinkedList_new();

  return newSignal;
}

void Signal_Fire(Signal* self, void* args) {
  LinkedList* connections = self->connections;
  Node* current = connections->front;

  while (current) {
    Connection* currentConn = current->entry;
    currentConn->callback(currentConn->context, args);

    if (currentConn->mode == LISTEN_ONCE) {
      Connection_Disconnect(currentConn);
    }
    current = current->next;
  }
}

Connection* Signal_Connect(Signal* self, SignalCallback callback, void* context) {
  Connection* newConn = (Connection*) malloc(sizeof(Connection));
  newConn->parent = self;
  newConn->connectionNode = LinkedList_insertEnd(self->connections, newConn);
  newConn->callback = callback;
  newConn->context = context;
  newConn->mode = NO_MODE;

  return newConn;
}

Connection* Signal_ConnectOnce(Signal* self, SignalCallback callback, void* context) {
  Connection* newConn = Signal_Connect(self, callback, context);
  newConn->mode = LISTEN_ONCE;

  return newConn;
}

void Signal_Free(Signal* self) {
  LinkedList_free(self->connections);
  free(self);
}

void Connection_Disconnect(Connection* self) {
  LinkedList_deleteNode(self->parent->connections, self->connectionNode);
  free(self);
}
