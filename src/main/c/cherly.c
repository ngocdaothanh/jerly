#include "cherly.h"
#include "common.h"

static void cherly_eject_callback(cherly_t *cherly, const char *key, int length);

void cherly_init(cherly_t *cherly, unsigned long max_size) {
  cherly->judy = judy_open(256);
  cherly->lru  = lru_create();
  cherly->size = 0;
  cherly->items_length = 0;
  cherly->max_size = max_size;
}

// node -> item -> value

void cherly_put(cherly_t *cherly, const char *key, int length, void *value, int size, DestroyCallback destroy) {
  judyslot * PValue;
  lru_item_t * item;
  
  dprintf("inserting with keylen %d vallen %d\n", length, size);
  PValue = judy_slot(cherly->judy, key, length);
  if (NULL != PValue) {
    item = (lru_item_t*)*PValue;
    dprintf("removing an existing value\n");
    cherly_remove(cherly, lru_item_key(item), lru_item_keylen(item));
  }
  
  if (cherly->size + size > cherly->max_size) {
    dprintf("projected new size %ld is more than max %ld\n", cherly->size + size, cherly->max_size);
    cherly->size -= lru_eject_by_size(cherly->lru, (length + size) - (cherly->max_size - cherly->size), (EjectionCallback)cherly_eject_callback, cherly);
  }
  
  item = lru_insert(cherly->lru, (char *) key, length, value, size, destroy);
  
  PValue = judy_cell(cherly->judy, key, length);
  *PValue = (judyslot)item;
  cherly->size += lru_item_size(item);
  dprintf("new cherly size is %ld\n", cherly->size);
  cherly->items_length++;
}

void * cherly_get(cherly_t *cherly, const char *key, int length) {
  judyslot * PValue;
  lru_item_t * item;
  
  PValue = judy_slot(cherly->judy, key, length);
  
  if (NULL == PValue) {
    return NULL;
  } else {
    item = (lru_item_t *)*PValue;
    lru_touch(cherly->lru, item);
    return lru_item_value(item);
  }
}

static void cherly_eject_callback(cherly_t *cherly, const char *key, int length) {
  judyslot *PValue;
  lru_item_t *item;
  
  PValue = judy_slot(cherly->judy, key, length);
  if (NULL == PValue) {
    return;
  }
  item = (lru_item_t*)*PValue;
  
  PValue = judy_del(cherly->judy);
  if (PValue) {
    cherly->items_length--;
    cherly->size -= lru_item_size(item);
  }
}

void cherly_remove(cherly_t *cherly, const char *key, int length) {
  judyslot *PValue;
  lru_item_t *item;
  
  PValue = judy_slot(cherly->judy, key, length);
  
  if (NULL == PValue) {
    return;
  }
  
  item = (lru_item_t *)*PValue;
  lru_remove_and_destroy(cherly->lru, item);
  cherly->size -= lru_item_size(item);
  cherly->items_length--;
  judy_del(cherly->judy);
}



void cherly_destroy(cherly_t *cherly) {
  dprintf("judy close\n");
  judy_close(cherly->judy);

  dprintf("lru destroy\n");
  lru_destroy(cherly->lru);
}