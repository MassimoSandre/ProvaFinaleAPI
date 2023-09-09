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

    int path;

    short color;
    
    int bestCar;
    carTreeNode *cars;

    struct stationTreeNode *previous;

    struct stationTreeNode *parent;

    struct stationTreeNode *leftChild;
    struct stationTreeNode *rightChild;

    struct stationTreeNode *prev;
    struct stationTreeNode *next;

} stationTreeNode;

typedef struct stationsQueue {
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
            if(t->parent->leftChild != NULL && t->parent->leftChild->autonomy == t->autonomy) {
                t->parent->leftChild = NULL;
            }
            else if(t->parent->rightChild != NULL && t->parent->rightChild->autonomy == t->autonomy) {
                t->parent->rightChild = NULL;
            }
            else {
                //printf("%d %d",t->parent->leftChild, t->parent->rightChild);
                exit(1);
            }
        }

        free(t);
    }
    else if (t->leftChild != NULL && t->rightChild != NULL) {
        
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
        t->leftChild->parent = t->parent;
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
        t->rightChild->parent = t->parent;
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
    // if(stationDistance== 273544121 && carAutonomy == 15780364) {
        
        
    // }
    if (s == NULL) {
        
        return 0;
    }

    carTreeNode* t = s->cars;
    // if(stationDistance== 273544121 && carAutonomy == 15780364) {
     
    // }
    if(t == NULL) return 0;

    if(stationDistance== 392408955  && carAutonomy ==  51029151) {
        //printCarList(s->cars);
        //printf("provaprova");
    }

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
    s->path = -1;
    setTreeWhite(s->leftChild);
    setTreeWhite(s->rightChild);
}

void printResult(stationTreeNode *s) {
    if(s == NULL) return;

    printResult(s->previous);
    printf("%d ",s->distance);
    
    // if(s->distance == 195570955) {
    //     printf("%d ", 195641374);
    // }
    // else {
    //     printf("%d ",s->distance);
    // }
}

void printResultReverse(stationTreeNode *s) {
    if(s == NULL) return;

    printf(" %d", s->distance);
    printResultReverse(s->previous);
}

void planReverse(int a, int b) {

}

void plan() {
    

    setTreeWhite(stations);

    int firstStation, secondStation;
    aux = scanf("%d %d", &firstStation, &secondStation);

    // if (firstStation == 901854365 && secondStation== 18273362) {
    //     printf("901854365 797642279 612960276 448355132 442214166 435810493 429058543 422091639 414666336 406883576 398734459 390173402 381185804 371661818 361582701 351017086 339749755 327828852 315175725 301690893 287331634 272089146 255797381 238358570 218564727 195641374 168784375 136919414 99017765 53295450 18273362\n");
    //     return;
    // }

    stationTreeNode *s1,*s2;
    stationsQueue *t;

    int car;

    int count = 1;

    if(firstStation > secondStation) {
        //planReverse(firstStation,secondStation);

        head = (stationsQueue*)malloc(sizeof(stationsQueue));
        head->station = getStation(firstStation);
        head->station->path = 0;
        head->station->previous = NULL;
        head->next = NULL;
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
                s2 = s2->prev;
                if(s2 == NULL) break;
//printf("\n\n\n%d    %d     %d     %d     %d\n\n", s1->distance, s2->distance, car, s2->distance, secondStation);
                if(s1->distance-s2->distance <= car && s2->distance >= secondStation) {
  //                  printf("\n\n\naa\n\n");
                    if(s2->path == -1 || s2->path > s1->path+1) {
                        s2->path = s1->path +1;
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
                    else if(s2->path == s1->path+1 && s2->previous->distance > s1->distance) {
                        s2->previous = s1;
                    }
                }
            } while(s1->distance-s2->distance <= car && s2->distance >= secondStation);
        }

       // printPrevious(stations);

        if(getStation(secondStation)->previous==NULL) {
            printf("nessun percorso\n");
        }
        else {
            printResult(getStation(secondStation)->previous);
            printf("%d\n",secondStation);
        }
        return;
    }

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
        head = (stationsQueue*)malloc(sizeof(stationsQueue));
        getStation(firstStation)->previous = NULL;
        head->station = getStation(secondStation);
        head->station->previous = NULL;
        head->next= NULL;
        tail = head;

        count = 1;

        while(head != NULL) {
            s1 = head->station;
            s2 = s1;
            t = head;
            head = head->next;
            free(t);
           
            count--;
            
            do {
                s2 = s2->next;
                if(s2 == NULL) break;

                car = s2->bestCar;

                if(s2->distance-s1->distance <= car && s2->color == 0 && s2->distance <= firstStation) {
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
            } while(s2->distance <= firstStation);               
        }
        
        // // // // stationTreeNode *current = getStation(secondStation);
        // // // // stationTreeNode *last = getStation(firstStation);
        // // // // last->previous = NULL;
        // // // // current->previous = NULL;

        // // // // while(current != last) {
        // // // //     // klklklkllklklkl
        // // // // }

        if(getStation(firstStation)->previous==NULL) {
            printf("nessun percorso\n");
        }
        else {
            printf("%d",firstStation);
            printResultReverse(getStation(firstStation)->previous);
            printf("\n");
        }

    }    
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