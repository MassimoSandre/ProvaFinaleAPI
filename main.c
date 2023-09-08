#include <stdlib.h>
#include <stdio.h>

typedef struct carTreeNode{
    int autonomy;

    struct carTreeNode* leftChild;
    struct carTreeNode* rightChild;
    
    struct carTreeNode* parent;
} carTreeNode;

typedef struct stationTreeNode{
    int distance;

    short color;
    
    int bestCar;
    carTreeNode *cars;

    int dist;

    struct stationTreeNode *previous;

    struct stationTreeNode *parent;

    struct stationTreeNode *leftChild;
    struct stationTreeNode *rightChild;

    struct stationTreeNode *prev;
    struct stationTreeNode *next;

    

} stationTreeNode;

typedef struct stationsQueue {
    int dist;
    stationTreeNode *station;

    struct stationsQueue *next;
} stationsQueue;

stationsQueue *head=NULL, *tail=NULL, *head2, *tail2, *temp;

stationTreeNode *stations = NULL;

int aux;

stationTreeNode* getStation(int distance);

stationTreeNode* getNextStation(stationTreeNode* s);

stationTreeNode* getPreviousStation(stationTreeNode* s);

int getBestCar(stationTreeNode* s);

int addStation();
int removeStation();
int addCar();
int removeCar();
void plan();

void nextCommand();



// ------------------ DEBUG ------------------
void printDist(stationTreeNode *s);
void printTreesDebug();
void printStations(stationTreeNode *s);
void printCarList(carTreeNode *s);
void printCars(stationTreeNode *s);
void printTreesDebug();
void printPrevious(stationTreeNode *s);
void printParents(stationTreeNode *s);
void check(stationTreeNode *s);
void printBestCars(stationTreeNode *s);
// --------------------------------------------


int main() {
    while (1) {
        if(feof(stdin)) break;
        
        nextCommand();
    }

    return 0;
}

stationTreeNode* getStation(int distance) {
    stationTreeNode* t = stations;

    if(t == NULL) return NULL;

    do {
        if (distance > t->distance) {
            t = t->rightChild;
        }
        else if (distance < t->distance) {
            t = t->leftChild;
        }

        if(t == NULL) return NULL;

    } while (distance != t->distance);

    return t;
}

stationTreeNode* getNextStation(stationTreeNode* s) {
    stationTreeNode* r = s;
    if(s->rightChild != NULL) {
        r = s->rightChild;
        while(r->leftChild != NULL) {
            r = r->leftChild;
        }
    }
    else {
        r = s->parent;
        while(r != NULL && r->distance < s->distance) r = r ->parent;
    }

    return r;
}

stationTreeNode* getPreviousStation(stationTreeNode* s) {
    stationTreeNode* r = s;
    if(s->leftChild != NULL) {
        r = s->leftChild;
        while(r->rightChild != NULL) {
            r = r->rightChild;
        }
    }
    else {
        r = s->parent;
        while(r != NULL && r->distance > s->distance) r = r ->parent;
    }

    return r;
}

int getBestCar(stationTreeNode* s) {
    if(s->cars == NULL) return -1;

    carTreeNode *c = s->cars;

    while(c->rightChild != NULL) {
        c = c->rightChild;
    }

    return c->autonomy;
}

void nextCommand() {
    char c,t='a';
    do {
        aux = scanf("%c", &c);
        if(feof(stdin)) break;
    } while (c < 97 || c > 122);

    if(feof(stdin)) return;

    if(c == 'a') {
        for(int i = 0; i < 9; i++) {
            aux = scanf("%c", &c);
        }
    }

    while(c != 'b' && t != ' ') {
        aux = scanf("%c", &t);
    }

    switch(c) {
        case 's':
            if(addStation()) {
                printf("aggiunta\n");
            }
            else{
                printf("non aggiunta\n");
            }
            break;
        case 'a':
            if(addCar()) {
                printf("aggiunta\n");
            }
            else{
                printf("non aggiunta\n");
            }
            break;
        case 'd':
            if(removeStation()) {
                printf("demolita\n");
            }
            else {
                printf("non demolita\n");
            }
            break;
        case 'r':
            if(removeCar()) {
                printf("rottamata\n");
            }
            else {
                printf("non rottamata\n");
            }
            break;
        case 'p':
            plan();
            break;

        case 'b':
            exit(1);
            printTreesDebug();

            break;

        default:
            printf("Comando non riconosciuto\n");
            break;

    }
}


int addStation() {
    int stationDistance, n, carAutonomy;
    aux = scanf("%d %d", &stationDistance, &n);

    if (getStation(stationDistance)) {
        for (int i = 0; i < n;i++) {
            aux = scanf("%d", &carAutonomy);
        }
    
        return 0;
    }

    stationTreeNode *newNode = (stationTreeNode*)malloc(sizeof(stationTreeNode));

    newNode->distance = stationDistance;
    newNode->cars = NULL;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->bestCar = -1;
    newNode->prev = NULL;
    newNode->next = NULL;

    carTreeNode *t;

    for (int i = 0; i < n;i++) {
        aux = scanf("%d", &carAutonomy);

        if(carAutonomy > newNode->bestCar) newNode->bestCar = carAutonomy;

        if(newNode->cars == NULL) {
            newNode->cars = (carTreeNode*)malloc(sizeof(carTreeNode));
            newNode->cars->autonomy = carAutonomy;
            newNode->cars->leftChild = NULL;
            newNode->cars->rightChild = NULL;
            newNode->cars->parent = NULL;
        }
        else {
            t = newNode->cars;

            int done = 0;
            while (!done) {
                if(carAutonomy < t->autonomy) {
                    if (t->leftChild == NULL) {
                        t->leftChild = (carTreeNode*)malloc(sizeof(carTreeNode));
                        t->leftChild->leftChild = NULL;
                        t->leftChild->rightChild = NULL;
                        t->leftChild->autonomy = carAutonomy;
                        t->leftChild->parent = t;
                        done = 1;
                    }
                    else {
                        t = t->leftChild;
                    }
                }
                else {
                    if (t->rightChild == NULL) {
                        t->rightChild = (carTreeNode*)malloc(sizeof(carTreeNode));
                        t->rightChild->leftChild = NULL;
                        t->rightChild->rightChild = NULL;
                        t->rightChild->autonomy = carAutonomy;
                        t->rightChild->parent = t;
                        done = 1;
                    }
                    else {
                        t = t->rightChild;
                    }
                }
            }
        }
    }

    if(stations == NULL) {
        stations = newNode;
        stations->parent = NULL;
        return 1;
    }

    stationTreeNode *u = stations;

    while(1) {
        if(stationDistance < u->distance) {
            if(u->leftChild == NULL) {
                u->leftChild = newNode;
                u->leftChild->parent = u;
                break;;
            }
            else {
                u = u->leftChild;
            }
        }
        else {
            if(u->rightChild == NULL) {
                u->rightChild = newNode;
                u->rightChild->parent = u;
                break;
            }
            else {
                u = u->rightChild;
            }
        }
    }

    newNode->next = getNextStation(newNode);    

    if(newNode->next != NULL) {
        newNode->prev = newNode->next->prev;
        newNode->next->prev = newNode;
    }
    else {
        newNode->prev = getPreviousStation(newNode);
    }

    if(newNode->prev != NULL)
        newNode->prev->next = newNode;

    return 1;
}

void deleteStationFromTree(stationTreeNode *s) {
    if(s->leftChild == NULL && s->rightChild == NULL) {
        if(s->prev != NULL) {
            s->prev->next = s->next;
        }
        if(s->next != NULL) {
            s->next->prev = s->prev;
        }

        if(s->parent == NULL) {
            free(s);
            stations = NULL;
        }
        else {
            if(s->parent->distance > s->distance) {
                s->parent->leftChild = NULL;
            }
            else {
                s->parent->rightChild = NULL;
            }

            free(s);
        }
    }
    else if (s->leftChild != NULL && s->rightChild != NULL) {
        stationTreeNode *t = s->next;
        s->cars = t->cars;
        s->distance = t->distance;
        s->bestCar = t->bestCar;
        s->next = t->next;

        deleteStationFromTree(t);

    }
    else if (s->leftChild != NULL && s->rightChild == NULL) {
        if(s->prev != NULL) {
            s->prev->next = s->next;
        }
        if(s->next != NULL) {
            s->next->prev = s->prev;
        }

        if(s->parent != NULL) {
            if(s->parent->distance > s->distance) {
                s->parent->leftChild = s->leftChild;
                s->leftChild->parent = s->parent;
            }
            else {
                s->parent->rightChild = s->leftChild;
                s->leftChild->parent = s->parent;
            }
        }
        else {
            stations = s->leftChild;
            stations->parent = NULL;
        }

        free(s);
    }
    else if (s->leftChild == NULL && s->rightChild != NULL) {
        
        if(s->prev != NULL) {
            s->prev->next = s->next;
        }
        if(s->next != NULL) {
            s->next->prev = s->prev;
        }

        if(s->parent != NULL) {
            if(s->parent->distance > s->distance) {
                s->parent->leftChild = s->rightChild;
                s->rightChild->parent = s->parent;
            }
            else {
                s->parent->rightChild = s->rightChild;
                s->rightChild->parent = s->parent;
            }
        }
        else {
            stations = s->rightChild;
            stations->parent = NULL;
        }

        free(s);
    }
}

int removeStation() {    
    int stationDistance;
    aux = scanf("%d",&stationDistance);

    stationTreeNode *s = getStation(stationDistance);
    if (s == NULL) {
        return 0;
    }

    deleteStationFromTree(s);

    return 1;
}

int addCar() {
    int stationDistance, carAutonomy;
    aux = scanf("%d %d", &stationDistance, &carAutonomy);

    stationTreeNode *s = getStation(stationDistance);
    if(s == NULL) {
        return 0;
    }

    if (s->bestCar < carAutonomy) s->bestCar = carAutonomy;

    carTreeNode *t = s->cars;

    if(t == NULL) {
        s->cars = (carTreeNode*)malloc(sizeof(carTreeNode));
        s->cars->leftChild = NULL;
        s->cars->rightChild = NULL;
        s->cars->parent = NULL;
        s->cars->autonomy = carAutonomy;

        return 1;
    }

    int done = 0;
    while (!done) {
        if(carAutonomy < t->autonomy) {
            if (t->leftChild == NULL) {
                t->leftChild = (carTreeNode*)malloc(sizeof(carTreeNode));
                t->leftChild->leftChild = NULL;
                t->leftChild->rightChild = NULL;
                t->leftChild->autonomy = carAutonomy;
                t->leftChild->parent = t;
                done = 1;
            }
            else {
                t = t->leftChild;
            }
        }
        else {
            if (t->rightChild == NULL) {
                t->rightChild = (carTreeNode*)malloc(sizeof(carTreeNode));
                t->rightChild->leftChild = NULL;
                t->rightChild->rightChild = NULL;
                t->rightChild->autonomy = carAutonomy;
                t->rightChild->parent = t;
                done = 1;
            }
            else {
                t = t->rightChild;
            }
        }
        
    }

    return 1;
}

void deleteCarFromTree(stationTreeNode* s, carTreeNode *t) {
    if(t->leftChild == NULL && t->rightChild == NULL) {
        if(t->parent == NULL) {
            s->cars = NULL;
        }
        else {
            if(t->parent->autonomy > t->autonomy) {
                t->parent->leftChild = NULL;
            }
            else {
                t->parent->rightChild = NULL;
            }
        }

        free(t);
    }
    else if (s->leftChild != NULL && s->rightChild != NULL) {
        carTreeNode* r = t;
        if(t->rightChild != NULL) {
            r = t->rightChild;
            while(r->leftChild != NULL) {
                r = r->leftChild;
            }
        }
        else {
            if (r->parent == NULL) return;
            while(r->parent->leftChild != r) {
                r = r->parent;

                if (r->parent == NULL) return;
            }
            r = r->parent;
        }

        t->autonomy = r->autonomy;
        
        deleteCarFromTree(s, r);
    }
    else if (t->leftChild != NULL && t->rightChild == NULL) {
        if(t->parent != NULL)  {
            if(t->parent->autonomy > t->autonomy) {
                t->parent->leftChild = t->leftChild;
            }
            else {
                t->parent->rightChild = t->leftChild;
            }
        }
        else {
            s->cars = t->leftChild;
        }

        free(t);
    }
    else if (t->leftChild == NULL && t->rightChild != NULL) {
        if(t->parent != NULL) {
            if(t->parent->autonomy > t->autonomy) {
                t->parent->leftChild = t->rightChild;
            }
            else {
                t->parent->rightChild = t->rightChild;
            }
        }
        else {
            s->cars = t->rightChild;
        }
        
        free(t);
    }
}

int removeCar() {
    int stationDistance, carAutonomy;
    aux = scanf("%d %d", &stationDistance, &carAutonomy);

    stationTreeNode *s = getStation(stationDistance);
    if (s == NULL) {
        return 0;
    }

    carTreeNode* t = s->cars;
    if(t == NULL) return 0;

    do {
        if (carAutonomy > t->autonomy) {
            
            t = t->rightChild;
        }
        else if (carAutonomy < t->autonomy) {
            t = t->leftChild;
        }

        if(t == NULL) return 0;
    } while (carAutonomy != t->autonomy);

    deleteCarFromTree(s,t);

    if(s->bestCar == carAutonomy) s->bestCar = getBestCar(s);

    return 1;
}


void setTreeWhite(stationTreeNode *s) {
    if(s == NULL) return;
    s->color = 0;
    s->previous = NULL;
    s->dist = -1;
    setTreeWhite(s->leftChild);
    setTreeWhite(s->rightChild);
}

void printResult(stationTreeNode *s) {
    if(s == NULL) return;

    printResult(s->previous);
    printf("%d ",s->distance);
}

void printResultReverse(stationTreeNode *s) {
    // if(s == NULL) return;

    // printf(" %d", s->distance);
    // printResultReverse(s->previous);

    //return;

    stationTreeNode *current = s;

    do {
        if(s->prev == NULL || s->prev->dist != s->dist) {
            printf("%d",s->distance);
            current = s;
            s = s->prev;
            if(s != NULL && s->dist != -1) printf(" ");
        }
        else if(current->distance-s->prev->distance > current->bestCar) {
            printf("%d ",s->distance);
            current = s;
            
            while(s->dist == current->dist) {
                s = s->prev;    
            }
        }
        else {
            s = s->prev;
        }
    } while(s->dist != -1);
}

void plan() {
    setTreeWhite(stations);

    int firstStation, secondStation;
    aux = scanf("%d %d", &firstStation, &secondStation);

    stationTreeNode *s1,*s2;
    stationsQueue *t;

    int car;

    int count = 1;

    // if(firstStation > secondStation) {
    //     car = firstStation;
    //     firstStation = secondStation;
    //     secondStation = car;
    // }

    if(firstStation < secondStation) {
        head = (stationsQueue*)malloc(sizeof(stationsQueue));
        head->station = getStation(firstStation);
        getStation(secondStation)->previous = NULL;
        head->station->previous = NULL;
        head->next= NULL;
        tail = head;

        while(head != NULL) {
            s1 = head->station;
            s2 = s1;
            t = head;
            head = head->next;
            free(t);

            count--;

            car = s1->bestCar;
            
            do {
                s2 = s2->next;
                if(s2 == NULL) break;

                if(s2->distance-s1->distance <= car && s2->color == 0 && s2->distance <= secondStation) {
                   
                    s2->color = 1;
                    s2->previous = s1;
                    if(count) {
                        tail->next = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
                        tail = tail->next;
                        tail->station = s2;
                        tail->next= NULL;
                    }
                    else {
                        head = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
                        head->station = s2;
                        head->next= NULL;
                        tail = head;
                    }   
                    count++;
                }
            } while(s2->distance-s1->distance <= car && s2->distance <= secondStation);
        }

        if(getStation(secondStation)->previous==NULL) {
            printf("nessun percorso\n");
        }
        else {
            printResult(getStation(secondStation)->previous);
            printf("%d\n",secondStation);
        }

    }
    else {
        
        
        // head = (stationsQueue*)malloc(sizeof(stationsQueue));
        // getStation(firstStation)->previous = NULL;
        // head->station = getStation(secondStation);
        // head->station->previous = NULL;
        // head->next= NULL;
        // tail = head;

        // count = 1;

        // while(head != NULL) {
        //     s1 = head->station;
        //     s2 = s1;
        //     t = head;
        //     head = head->next;
        //     free(t);
           
        //     count--;
            
        //     do {
        //         s2 = s2->next;
        //         if(s2 == NULL) break;

        //         car = s2->bestCar;

        //         if(s2->distance-s1->distance <= car && s2->color == 0 && s2->distance <= firstStation) {
        //             s2->color = 1;
        //             s2->previous = s1;
        //             if(count) {
        //                 tail->next = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
        //                 tail = tail->next;
        //                 tail->station = s2;
        //                 tail->next= NULL;
        //             }
        //             else {
        //                 head = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
        //                 head->station = s2;
        //                 head->next= NULL;
        //                 tail = head;
        //             }   
        //             count++;
        //         }
        //     } while(s2->distance <= firstStation);               
        // }

        // if(getStation(firstStation)->previous==NULL) {
        //     printf("nessun percorso\n");
        // }
        // else {
        //     printf("%d",firstStation);
        //     printResultReverse(getStation(firstStation)->previous);
        //     printf("\n");
        // }

        // stationTreeNode *current = getStation(firstStation);
        // stationTreeNode *previous = current;
        // current = current->prev;

        // stationTreeNode *t;

        
        // //current = current->prev;

        // //last->previous = NULL;
        // //current->previous = NULL;
        // if(current != NULL) {
        //     do {
                
        //         if(previous->distance-current->distance > current->bestCar) {
        //             // non c'è un percorso
        //             break;
        //         }

        //         t = current->next;
        //         while(t!=NULL && t->distance <= firstStation && t->distance-current->distance <= current->bestCar) {
        //             current->previous = t;
        //             t = t->previous;
        //         }

        //         previous = current;
        //         current = current->prev;

        //     }while(current != NULL && current->distance >= secondStation);
        //     //printf("\n\n%d    %d    %d \n\n", previous->distance, current->distance, current->distance);
        // }

        // ===============================================================

        // head = (stationsQueue*)malloc(sizeof(stationsQueue));
        // getStation(secondStation)->previous = NULL;
        // head->station = getStation(firstStation);
        // head->station->previous = NULL;
        // head->next= NULL;
        // tail = head;

        // int count2 = 1;


        // while(head != NULL) {
        //     s1 = head->station;
        //     s2 = s1;
        //     t = head;
        //     head = head->next;
        //     free(t);
        //     count2--;
            

        //     car = getBestCar(s1);
            
            

        //     head2 = NULL;
        //     tail2 = NULL;
        //     count = 0;

            
            
        //     do {
                
        //         s2 = getPreviousStation(s2);
                

                
              
        //         if(s2 == NULL) break;
        //         if(s1->distance-s2->distance <= car && s2->color == 0 && s2->distance >= secondStation) {
        //             s2->color = 1;
        //             s2->previous = s1;
        //             if(count) {
        //                 temp = (stationsQueue*)malloc(sizeof(stationsQueue));
        //                 // temp->next = head2;

        //                 // head2 = temp;
        //                 // head2->station = s2;
        //                 temp->next = NULL;

        //                 tail2->next = temp;
        //                 tail2 = tail2->next;
        //                 tail2->station = s2;
        //             }
        //             else {
        //                 head2 = (stationsQueue*)malloc(sizeof(stationsQueue));
        //                 // head2->station = s2;
        //                 // head2->next= NULL;
        //                 // tail2 = head2;

        //                 head2->station = s2;
        //                 head2->next= NULL;
        //                 tail2 = head2;
        //             }   
        //             count++;
        //         }

                

        //     } while(s1->distance-s2->distance <= car && s2->distance >= secondStation);
            
                
        //     if(count) {
        //         if(count2) {
        //             tail->next = head2;
        //             tail = tail2;
        //         }
        //         else {
        //             head = head2;
        //             tail = tail2;

        //         }
        //     }

        //     count2+=count;

            
        // }

        // ================================================

        // head = (stationsQueue*)malloc(sizeof(stationsQueue));
        // getStation(firstStation)->previous = NULL;
        // head->station = getStation(secondStation);
        // head->station->previous = NULL;
        // head->next= NULL;
        // tail = head;

        // count = 1;


        // while(head != NULL) {
        //     s1 = head->station;
        //     s2 = s1;
        //     t = head;
        //     head = head->next;
        //     free(t);
        //     count2--;
        //     count--;
            
        //     head2 = NULL;
        //     tail2 = NULL;
        //     count = 0;

            
            
        //     do {
                
        //         s2 = s2->next;//getNextStation(s2);
                
        //         if(s2 == NULL) break;

        //         car = s2->bestCar;

        //         if(s2->distance == 95 ) {

                    
        //             //check(stations);
        //             printf("\n\n ccccccccccccccc\n");
        //             printStations(stations);



        //             printf("\nbbbbbbbbbbbbbb\n\n");
        //             stationTreeNode *r,*s = stations;
        //             while(s->leftChild != NULL) s = s->leftChild;
        //             while(s != NULL) {
        //                 printf("%d\t",s->distance);
        //                 s = getNextStation(s);

        //                 // if(s->distance == 355) {
        //                 //     r = s->parent;
        //                 //     printf("\n\n%x   %x  %x  %x\n\n", s,s->parent, r->leftChild, r->rightChild);
        //                 // }
        //             }
        //             printf("\naaaaaaaaaa\n\n");

        //             printf("\n\n\n");
        //             s = stations;
        //             while(s != NULL) {
        //                 printf("%d\t",s->distance);
        //                 s = getPreviousStation(s);

        //                 if(s->distance == 1395) {
        //                     printf("%x  %x  %x", s->parent, s->leftChild, s->rightChild);
        //                 }
        //             }

        //             printParents(stations);
        //         }


        //         if(s2->distance-s1->distance <= car && s2->color == 0 && s2->distance <= firstStation) {
        //             s2->color = 1;
        //             s2->previous = s1;
        //             if(count) {
        //                 tail->next = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
        //                 tail = tail->next;
        //                 tail->station = s2;
        //                 tail->next= NULL;
        //             }
        //             else {
        //                 head = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
        //                 head->station = s2;
        //                 head->next= NULL;
        //                 tail = head;
        //             }   
        //             count++;
        //         }

                

        //     } while(/*s2->distance-s1->distance <= car &&*/ s2->distance <= firstStation);+

        head = (stationsQueue*)malloc(sizeof(stationsQueue));
        head->station = getStation(firstStation);
        getStation(secondStation)->previous = NULL;
        head->station->previous = NULL;
        head->next= NULL;
        tail = head;

        head->dist = 0;

        while(head != NULL) {
            s1 = head->station;
            s2 = s1;
            t = head;
            s1->dist = t->dist;
            head = head->next;
            free(t);
            

            count--;

            car = s1->bestCar;
            
            do {
                s2 = s2->prev;
                if(s2 == NULL) break;

                if(-s2->distance+s1->distance <= car && s2->color == 0 && s2->distance >= secondStation) {
                   
                    s2->color = 1;
                    s2->previous = s1;
                    if(count) {
                        tail->next = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
                        tail = tail->next;
                        tail->station = s2;
                        tail->next= NULL;
                        tail->dist = s1->dist+1;
                    }
                    else {
                        head = (stationsQueue*)malloc(sizeof(stationsQueue));
                        
                        head->station = s2;
                        head->next= NULL;
                        tail = head;
                        tail->dist = s1->dist+1;
                    }   
                    count++;
                }
            } while(-s2->distance+s1->distance <= car && s2->distance >= secondStation);
        }

        //printDist(stations);

        if(/*getStation(secondStation)->previous==NULL*/ getStation(secondStation)->dist == -1) {
            printf("nessun percorso\n");
        }
        else {
            printResultReverse(getStation(firstStation));
            printf("\n");
        }

    
        
    }    
}


// ------------------ DEBUG ------------------
void printDist(stationTreeNode *s) {
    if(s == NULL) return;
    printDist(s->leftChild);
    
    printf("\t%d - %d\n",s->distance, s->dist);
    
    printDist(s->rightChild);
}

void printPrevious(stationTreeNode *s) {
    if(s == NULL) return;
    printPrevious(s->leftChild);
    if(s->previous == NULL) {
        printf("\t%d - %d\t\t\t%d\n",s->distance, -1, getBestCar(s));
    }else {
        printf("\t%d - %d\t\t\t%d\n",s->distance, s->previous->distance,getBestCar(s));
    }
    
    printPrevious(s->rightChild);

}

void printParents(stationTreeNode *s) {
    if(s == NULL) return;
    printParents(s->leftChild);
    if(s->parent == NULL) {
        printf("\t0");
    }
    else {
        printf("\t%d",s->parent->distance);
        }
    printParents(s->rightChild);
    
}

void check(stationTreeNode *s) {
    if(s != stations) {
        if(s == NULL) return;

        if(s != s->parent->leftChild && s != s->parent->rightChild) {
            printf("\n\nERRORE\n\n");

            printf("\n\n%d    %d\n\n", s->distance, s->parent->distance);
        }
    }
    
    check(s->leftChild);
    check(s->rightChild);
}

void printBestCars(stationTreeNode *s) {
    if(s == NULL) return;

    printBestCars(s->leftChild);

    printf("%d\n", getBestCar(s)- s->bestCar);

    
    printBestCars(s->rightChild);
}

void printStations(stationTreeNode *s) {
    if(s == NULL) return;

    printStations(s->leftChild);
    printf("%d\t",s->distance);
    printStations(s->rightChild);
}

void printCarList(carTreeNode *s) {
    if(s == NULL) return;


    printCarList(s->leftChild);
    printf("%d\t",s->autonomy);
    printCarList(s->rightChild);
}

void printCars(stationTreeNode *s) {
    if(s == NULL) return;

    printCars(s->leftChild);
    printf("\n");
    printCarList(s->cars);
    printCars(s->rightChild);
}

void printTreesDebug() {
    printf("Stazioni:\n\t");
    printStations(stations);
    printf("\n\nAuto:");

    printCars(stations);

    printf("\n");

}
// -------------------------------------------