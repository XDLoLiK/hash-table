/**
 * @defgroup   LIST list
 *
 * @brief      This file implements list.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "list.h"

struct list *list_new(const char *key, const char *value)
{
	struct list *new_list = (struct list *)calloc(1, sizeof (struct list));
	if (new_list == NULL) {
		return new_list;
	}

	new_list->key   = (char *)aligned_alloc(32, 32 * sizeof (char));
	new_list->value = (char *)aligned_alloc(32, 32 * sizeof (char));
	if (new_list->key == NULL || new_list->value == NULL) {
		list_free_node(new_list);	
		return NULL;
	}

	_mm256_store_si256((__m256i *)new_list->key,   *(__m256i *)key);
	_mm256_store_si256((__m256i *)new_list->value, *(__m256i *)value);
	new_list->next  = NULL;
	return new_list;
}

struct list *list_insert(struct list *head, const char *key, const char *value)
{
	if (head == NULL) {
		return list_new(key, value);
	}

	struct list *current_list = head;
	while (current_list->next) {
		current_list = current_list->next;
	}
	current_list->next = list_new(key, value);
	return head;
}

struct list *list_find(struct list *head, const char *key)
{
	struct list *current_list = head;
	while (current_list) {
		if (strcmp32_avx2(*(__m256i *)current_list->key, *(__m256i *)key) == 0) {
			return current_list;
		}
		current_list = current_list->next;
	}

	return NULL;
}

struct list *list_erase(struct list *head, const char *key)
{
	if (head == NULL) {
		return NULL;
	}

	if (strcmp32_avx2(*(__m256i *)head->key, *(__m256i *)key) == 0) {
		struct list *new_head = head->next;
		list_free_node(head);
		return new_head;
	}

	struct list *current_list = head;
	while (current_list->next) {
		if (strcmp32_avx2(*(__m256i *)current_list->next->key, *(__m256i *)key) == 0) {
			struct list* new_next = current_list->next->next;
			list_free_node(current_list->next);
			current_list->next = new_next;
			break;
		}
		current_list = current_list->next;
	}

	return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
	if (where == NULL || what == NULL) return head;
	if (head  == NULL)                 return what;

	struct list *temp = where->next;
	where->next = what;
	what->next  = temp;

	return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
	if (where == NULL || what == NULL) return head;
	if (head  == NULL)                 return what;

	if (where == head) {
		what->next = where;
		return what;
	}

	struct list *current_list = head;
	while (current_list->next) {
		if  (current_list->next == where) {
			struct list* temp = current_list->next;
			current_list->next = what;
			what->next = temp;
			break;
		}
		current_list = current_list->next;
	}

	return head;
}

struct list *list_delete(struct list *head)
{
	struct list *current_list = head;
	while(current_list) {
		struct list *next_list = current_list->next;
		list_free_node(current_list);
		current_list = next_list;
	}

	return NULL;
}

struct list *list_next(struct list *cur)
{
	if (cur == NULL) return NULL;
	return cur->next;
}

struct list *list_free_node(struct list *node)
{
	free(node->key);
	free(node->value);
	free(node);
	return NULL;
}

void list_print(struct list const *head)
{
	printf("[");

	struct list const *current_list = head;
	while(current_list) {
		printf("{key:\"%s\", value:\"%s\"}", current_list->key, current_list->value);
		if (current_list->next) printf(", ");
		current_list = current_list->next;
	}	

	printf("];\n");
	return;
}
