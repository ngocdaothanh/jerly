#include <time.h>

#include "cherly.h"
#include "common.h"

static void cherly_eject_callback(cherly_t *cherly, const char *key, int length) {
  judyslot *PValue;
  lru_item_t *item;

  PValue = judy_slot(cherly->judy, key, length);
  if (NULL == PValue) return;

  item = (lru_item_t *) *PValue;
  if (NULL == item) return;

  dprintf("cherly_eject_callback, key: %s\n", key);
  cherly->items_length--;
  cherly->size -= lru_item_size(item);

  judy_del(cherly->judy);
}

void cherly_init(cherly_t *cherly, unsigned long max_size) {
  cherly->judy = judy_open(512);
  cherly->lru  = lru_create();
  cherly->size = 0;
  cherly->items_length = 0;
  cherly->max_size = max_size;
}

// node -> item -> value

void cherly_put(cherly_t *cherly, const char *key, int length, void *value, int size, int ttl, DestroyCallback destroy) {
  judyslot *PValue;
  lru_item_t *item;
  
  if (cherly->size + length + size > cherly->max_size) {
    cherly->size -= lru_eject_by_size(cherly->lru, (length + size) - (cherly->max_size - cherly->size), (EjectionCallback)cherly_eject_callback, cherly);
  }

  PValue = judy_slot(cherly->judy, key, length);
  if (NULL != PValue) {
    item = (lru_item_t*) *PValue;
    if (NULL != item) {    
      dprintf("removing an existing value\n");
      cherly_remove(cherly, lru_item_key(item), lru_item_keylen(item));
    }
  }

  dprintf("inserting with key: %s, vallen: %d\n", key, size);

  item = lru_insert(cherly->lru, (char *) key, length, value, size, ttl, destroy);
  
  PValue = judy_cell(cherly->judy, key, length);
  *PValue = (judyslot) item;
  cherly->size += lru_item_size(item);
  dprintf("new cherly size is %ld\n", cherly->size);
  cherly->items_length++;
}

void * cherly_get(cherly_t *cherly, const char *key, int length) {
  judyslot *PValue;
  lru_item_t *item;

  PValue = judy_slot(cherly->judy, key, length);
  if (NULL == PValue) return NULL;

  item = (lru_item_t *) *PValue;
  if (NULL == item) return NULL;

  if (item->ttl > 0) {
    int now = time(NULL);
    if (now - item->touched_at > item->ttl) {
      lru_remove_and_destroy(cherly->lru, item);
      cherly->size -= lru_item_size(item);
      cherly->items_length--;
    
      judy_del(cherly->judy);
      return NULL;
    }
  }

  lru_touch(cherly->lru, item);
  return lru_item_value(item);
}

void cherly_remove(cherly_t *cherly, const char *key, int length) {
  judyslot *PValue;
  lru_item_t *item;

  PValue = judy_slot(cherly->judy, key, length);
  if (NULL == PValue) return;
  
  item = (lru_item_t *) *PValue;
  if (NULL == item) return;

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
