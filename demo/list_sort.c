/*********************************************
 * Filename: list_sort.c
 * Author: PIzhou
 * Mail: yuanzhou@outlook.com
 * CreateTime: 2014年03月24日 星期一 09时20分05秒
 *********************************************/

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../src/list.h"


struct marathon_result{

    char player[100];
    int result_in_sec;
    struct list_head node;
};

void add_to_list(struct marathon_result* mr, struct list_head* header) {
    INIT_LIST_HEAD(&mr->node);
    list_add_tail(&mr->node, header);
}

static int cmp_marathon_result(const void *p1, const void *p2) {
    struct marathon_result *mr1 = (struct marathon_result *)list_entry(p1, struct marathon_result, node);
    struct marathon_result *mr2 = (struct marathon_result *)list_entry(p2, struct marathon_result, node);
    return mr1->result_in_sec > mr2->result_in_sec;
}

void insertion_sort(struct list_head* header, int(*compar)(const void *, const void *)) {

    if (!prefetch(header) || NULL == compar) {
        return ;
    }

    struct list_head *list_node;
    struct list_head *next_list_node;
    list_for_each_safe(list_node, next_list_node, header) {
        struct list_head *sorted_list_node;
        list_for_each(sorted_list_node, header) {
            if (sorted_list_node == list_node) {
                break;
            }

            if (compar(sorted_list_node, list_node)) {
                list_del(list_node);
                list_add_tail(list_node, sorted_list_node);
                break;
            }
        }
    }
    return;
}

void quick_sort_recursion(struct list_head* begin, struct list_head* end, int(*compar)(const void *, const void *)) {
    
    if (!prefetch(begin) || !prefetch(end) || begin->next == end || NULL == compar) {
        return ;
    }

    struct list_head *div_list_node = NULL;
    struct list_head *list_node;
    struct list_head *next_list_node;
    list_for_each_safe(list_node, next_list_node, begin) {
        if (list_node == end) {
            break;
        }
        if (NULL == div_list_node) {
            div_list_node = list_node;
            continue;
        }
        if (compar(div_list_node, list_node)) {
            list_del(list_node);
            list_add_tail(list_node, div_list_node);
        }
    }

    if (NULL == div_list_node) {
        return;
    }

    quick_sort_recursion(begin, div_list_node, compar);
    quick_sort_recursion(div_list_node, end, compar);
}

void quick_sort(struct list_head* header, int(*compar)(const void *, const void *)) {
    quick_sort_recursion(header, header, compar);
    return;
}

void test_correct() {

    struct list_head insertion_header;
    INIT_LIST_HEAD(&insertion_header);
    struct list_head quick_header;
    INIT_LIST_HEAD(&quick_header);

    srand((unsigned)time(NULL));

    int i = 0;
    for (i = 0; i < 10; i++) {
        struct marathon_result* mr1 = (struct marathon_result*)malloc(sizeof(struct marathon_result));
    
        if (NULL == mr1) {
            printf("malloc error\n");
            return ;
        }

        memset(mr1, 0x0, sizeof(struct marathon_result));

        mr1->player[0] = 'A' + i;
        mr1->result_in_sec = rand() % 10000;
        
        struct marathon_result* mr2 = (struct marathon_result*)malloc(sizeof(struct marathon_result));
    
        if (NULL == mr2) {
            printf("malloc error\n");
            return ;
        }

        memcpy(mr2, mr1, sizeof(struct marathon_result));

        add_to_list(mr1, &insertion_header);
        add_to_list(mr2, &quick_header);
    }

    struct marathon_result *mr;
    printf("raw list:\n");
    list_for_each_entry(mr, &insertion_header, node) {
        printf("%s:%d ", mr->player, mr->result_in_sec);
    }
    printf("\n");

    insertion_sort(&insertion_header, cmp_marathon_result);
    quick_sort(&quick_header, cmp_marathon_result);

    struct list_head *list_node;
    struct list_head *next_list_node;
    printf("insertion_sort list:\n");
    list_for_each_safe(list_node, next_list_node, &insertion_header) {
        list_del(list_node);
        mr = list_entry(list_node, struct marathon_result, node);
        printf("%s:%d ", mr->player, mr->result_in_sec);
        free(mr);
    }
    printf("\n");
    printf("quick_header list:\n");
    list_for_each_safe(list_node, next_list_node, &quick_header) {
        list_del(list_node);
        mr = list_entry(list_node, struct marathon_result, node);
        printf("%s:%d ", mr->player, mr->result_in_sec);
        free(mr);
    }
    printf("\n");
    return ;
}

void test_performance() {

    struct list_head insertion_header;
    INIT_LIST_HEAD(&insertion_header);
    struct list_head quick_header;
    INIT_LIST_HEAD(&quick_header);

    srand((unsigned)time(NULL));

    int i = 0;
    for (i = 0; i < 100000; i++) {
        struct marathon_result* mr1 = (struct marathon_result*)malloc(sizeof(struct marathon_result));
    
        if (NULL == mr1) {
            printf("malloc error\n");
            return ;
        }

        memset(mr1, 0x0, sizeof(struct marathon_result));

        mr1->player[0] = 'A' + i;
        mr1->result_in_sec = rand() % 10000;
        
        struct marathon_result* mr2 = (struct marathon_result*)malloc(sizeof(struct marathon_result));
    
        if (NULL == mr2) {
            printf("malloc error\n");
            return ;
        }

        memcpy(mr2, mr1, sizeof(struct marathon_result));

        add_to_list(mr1, &insertion_header);
        add_to_list(mr2, &quick_header);
    }

    printf("100000 performance:\n");

    struct timespec tpstart;
    struct timespec tpend;

    clock_gettime(CLOCK_REALTIME, &tpstart);
    insertion_sort(&insertion_header, cmp_marathon_result);
    clock_gettime(CLOCK_REALTIME, &tpend);
    double timedif = (tpend.tv_sec-tpstart.tv_sec)+(tpend.tv_nsec-tpstart.tv_nsec)/1000000000.0;
    printf("insertion_sort time: %.12fs\n", timedif);
    
    clock_gettime(CLOCK_REALTIME, &tpstart);
    quick_sort(&quick_header, cmp_marathon_result);
    clock_gettime(CLOCK_REALTIME, &tpend);
    timedif = (tpend.tv_sec-tpstart.tv_sec)+(tpend.tv_nsec-tpstart.tv_nsec)/1000000000.0;
    printf("quick_sort time: %.12fs\n", timedif);

    struct list_head *insertion_node;
    struct list_head *quick_node;

	for (insertion_node = insertion_header.next, quick_node = quick_header.next; 
            prefetch(insertion_node->next) && insertion_node != &insertion_header && prefetch(quick_node->next) && quick_node != &quick_header; 
        	insertion_node = insertion_node->next, quick_node = quick_node->next) {
        struct marathon_result *mr1 = list_entry(insertion_node, struct marathon_result, node);
        struct marathon_result *mr2 = list_entry(quick_node, struct marathon_result, node);
        if (mr2->result_in_sec != mr1->result_in_sec) {
            printf("not correct");
            break;
        }
    }

    struct marathon_result *mr;
    struct list_head *list_node;
    struct list_head *next_list_node;

    list_for_each_safe(list_node, next_list_node, &insertion_header) {
        list_del(list_node);
        mr = list_entry(list_node, struct marathon_result, node);
        free(mr);
    }
    list_for_each_safe(list_node, next_list_node, &quick_header) {
        list_del(list_node);
        mr = list_entry(list_node, struct marathon_result, node);
        free(mr);
    }
}

int main(int argc, char *argv[]) {
    test_correct();
    test_performance();
    return 1;
}
