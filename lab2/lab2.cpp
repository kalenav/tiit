#include <iostream>
#include <stdio.h>

struct City
{
    short citynum, adjacentqty;
    short* adjacentstati;
    City** adjacent;
    bool checked = false;
};

struct Road
{
    short city1, city2, status;
};

struct LinkStackEl
{
    short* link;
    LinkStackEl* next;
};

struct RoadStackEl
{
    short roadnum;
    RoadStackEl* next;
};

void linkStackPush(LinkStackEl** head, short adjcity1, short adjcity2)
{
    LinkStackEl* lstemp = new LinkStackEl;
    lstemp->link = new short[2];
    lstemp->link[0] = adjcity1;
    lstemp->link[1] = adjcity2;
    lstemp->next = *head;
    (*head) = lstemp;
}

void linkStackPop(LinkStackEl** head)
{
    LinkStackEl* lstemp = *head;
    (*head) = (*head)->next;
    delete lstemp;
}

short* linkStackSearch(LinkStackEl* head, short el1, short el2)
{
    LinkStackEl* lstemp = head;
    while (lstemp != NULL)
    {
        if ((lstemp->link[0] == el1 && lstemp->link[1] == el2) || (lstemp->link[1] == el1 && lstemp->link[0] == el2)) return lstemp->link;
        lstemp = lstemp->next;
    }
    return NULL;
}

void roadStackPush(RoadStackEl** head, short input)
{
    RoadStackEl* lstemp = new RoadStackEl;
    lstemp->roadnum = input;
    lstemp->next = *head;
    (*head) = lstemp;
}

void roadStackPrint(RoadStackEl* head, FILE* fp)
{
    RoadStackEl* lstemp = head;
    while (lstemp->next != NULL)
    {
        fprintf(fp, "%d ", lstemp->roadnum);
        lstemp = lstemp->next;
    }
    return;
}

int checksum(City* currcity, LinkStackEl* linkstoomit, short status)
{
    short sum = currcity->citynum;
    currcity->checked = true;
    for (short curr = 0; curr < currcity->adjacentqty; curr++)
    {
        City* nextcity = currcity->adjacent[curr];
        if(linkStackSearch(linkstoomit, currcity->citynum, nextcity->citynum) != NULL) goto next;
        if (nextcity->checked == false && (currcity->adjacentstati[curr] == status || currcity->adjacentstati[curr] == 3)) sum += checksum(nextcity, linkstoomit, status);
        next:;
    }
    return sum;
}

void renew(City** cities, short cityqty)
{
    for (short i = 1; i <= cityqty; i++) cities[i]->checked = false;
    return;
}

void removelinks(City** cities, Road* roads, short cityqty, short roadqty, LinkStackEl* lto, LinkStackEl* ltk, int tempsum, FILE* fp)
{
    short removedqty = 0;
    RoadStackEl* removedroads = new RoadStackEl;
    removedroads->next = NULL;
    removedroads->next = 0;
    again:
    for (short currc = 1; currc <= cityqty; currc++)
    {
        City* currcity = cities[currc];
        for (short curr = 0; curr < currcity->adjacentqty; curr++)
        {
            short tempcitynum = currcity->adjacent[curr]->citynum;
            if (linkStackSearch(ltk, currcity->citynum, tempcitynum) != NULL) continue;
            linkStackPush(&lto, currcity->citynum, tempcitynum);
            if (checksum(currcity, lto, 1) != tempsum)
            {
                linkStackPop(&lto);
                linkStackPush(&ltk, currcity->citynum, tempcitynum);
                renew(cities, cityqty);
                continue;
            }
            renew(cities, cityqty);
            if (checksum(currcity, lto, 2) != tempsum)
            {
                linkStackPop(&lto);
                linkStackPush(&ltk, currcity->citynum, tempcitynum);
                renew(cities, cityqty);
                continue;
            }
            for (short curoad = 0; curoad < roadqty; curoad++)
            {
                if ((roads[curoad].city1 == currcity->citynum && roads[curoad].city2 == tempcitynum) || (roads[curoad].city2 == currcity->citynum && roads[curoad].city1 == tempcitynum))
                {
                    removedqty++;
                    roadStackPush(&removedroads, curoad+1);
                }
            }
            goto again;
        }
    }
    fprintf(fp, "%d\n", removedqty);
    roadStackPrint(removedroads, fp);
    return;
}

int main()
{
    int cityqty = 0;
    int roadqty = 0;
    FILE* fp;
    fopen_s(&fp, "E:/Important/tiit/input.txt", "r");
    fscanf_s(fp, "%d %d\n", &cityqty, &roadqty);
    if (roadqty == 0 || cityqty == 0)
    {
        fopen_s(&fp, "E:/Important/tiit/output.txt", "w");
        fprintf(fp, "-1");
        return 0;
    }
    Road* roads = new Road [roadqty];
    for (short curr = 0; curr < roadqty; curr++)
    {
        fscanf_s(fp, "%d %d %d\n", &(roads[curr].city1), &(roads[curr].city2), &(roads[curr].status));
        if (roads[curr].city1 > roads[curr].city2)
        {
            short temp = roads[curr].city1;
            roads[curr].city1 = roads[curr].city2;
            roads[curr].city2 = temp;
        }
    }
    City** cities = new City* [cityqty+1];
    for (short curr = 1; curr <= cityqty; curr++)
    {
        City* tempcity = new City;
        tempcity->adjacent = new City* [cityqty];
        tempcity->adjacentqty = 0;
        tempcity->checked = false;
        tempcity->citynum = curr;
        tempcity->adjacentstati = new short [cityqty];
        cities[curr] = tempcity;
    }
    for (short curr = 1; curr <= cityqty; curr++)
    {
        for (short curoad = 0; curoad < roadqty; curoad++)
        {
            short firstcitynum = roads[curoad].city1;
            short secondcitynum = roads[curoad].city2;
            short currstatus = roads[curoad].status;
            if (firstcitynum == curr)
            {
                cities[curr]->adjacent[cities[curr]->adjacentqty] = cities[secondcitynum];
                cities[curr]->adjacentstati[cities[curr]->adjacentqty++] = currstatus;
            }
            if (secondcitynum == curr)
            {
                cities[curr]->adjacent[cities[curr]->adjacentqty] = cities[firstcitynum];
                cities[curr]->adjacentstati[cities[curr]->adjacentqty++] = currstatus;
            }
        }
    }
    LinkStackEl* head_linkstoomit = new LinkStackEl;
    LinkStackEl* head_linkstokeep = new LinkStackEl;
    head_linkstoomit->link = head_linkstokeep->link = new short[2];
    head_linkstoomit->link[0] = head_linkstokeep->link[0] = 0;
    head_linkstoomit->link[1] = head_linkstokeep->link[1] = 0;
    head_linkstoomit->next = head_linkstokeep->next = NULL;
    int mastersum = cityqty * (cityqty + 1) / 2;
    int tempsum = checksum(cities[1], head_linkstoomit, 1);
    if (mastersum != tempsum) return 0;
    renew(cities, cityqty);
    fopen_s(&fp, "E:/Important/tiit/output.txt", "w");
    removelinks(cities, roads, cityqty, roadqty, head_linkstokeep, head_linkstoomit, tempsum, fp);
    return 0;
}
