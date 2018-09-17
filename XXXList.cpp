// datei: xxxlist.cpp

/*
 *	Disclaimer: Eine EINFACHE verkettete Liste (zB: für die Laserschüsse)
 */

// INCLUDES
#include<stdio.h>
#include<stdlib.h>
#include"XXXGlobal.h"

// GLOBALS

	// keine

// FUNKTIONEN

// Liste erzeugen mit RAM Initialisierung
XXX_RESULT xxx_List_Create(XXXList **pp_list)
{
	*pp_list = (XXXList*)malloc(sizeof(XXXList));
	if(*pp_list == NULL)
		return XXX_MEMORY_ERROR;

	(*pp_list)->data = NULL;
	(*pp_list)->next = NULL;
	return XXX_OK;
}

// Einfügen am Anfang der Liste
XXX_RESULT xxx_List_Insert(XXXList **pp_list,void *p_data)
{
	XXXList *pNewelement;		// neues Listenelement

	// 1.Fall: Einfügen in leere Liste
	if((*pp_list)->data ==NULL)
	{
		(*pp_list)->data = p_data;
		return XXX_OK;
	}

	// 2.Fall: Einfügen in icht leere Liste
	pNewelement = (XXXList*)malloc(sizeof(XXXList));
	// Fehler bei Initialisierung
	if(pNewelement == NULL)
		return XXX_MEMORY_ERROR;
	else
	{	// Daten speichern, Zeiger setzen
		pNewelement->data = p_data;
		pNewelement->next = *pp_list;
		*pp_list = pNewelement;
		return XXX_OK;
	}
}//x


// Löschen an angegebener Position,Start bei Index 0
XXX_RESULT xxx_List_Delete(XXXList **pp_list,int index)
{
	XXXList *temp,*prev;

	// 1.Fall: Leere Liste
	if((*pp_list == NULL) || ((*pp_list)->data ==NULL))
		return XXX_OUT_OF_BOUNDS;

	// 2.Fall: Erstes Element löschen
	if(index == 0)
	{
		// erstes Element übergeben und Speicher freigeben
		temp = *pp_list;
		*pp_list = (*pp_list)->next;
		free(temp);
		return XXX_OK;
	}

	// 3.Fall: 2,3,... Element löschen
	temp = *pp_list;

	// Liste durchlaufen bis Index erreicht oder Liste zu Ende
	while(((*pp_list)->next != NULL) && (index != 0))
	{
		// vorgängerelement merken und weitergeben
		prev = *pp_list;
		*pp_list = (*pp_list)->next;
		index--;
	}

	// Index wurde nicht gefunden -> Liste zu kurz
	if(index != 0)
	{
		*pp_list = temp;
		return XXX_OUT_OF_BOUNDS;
	}

	// Gefundenes Element übergeben und Speicher freigeben
	prev->next = (*pp_list)->next;
	free(*pp_list);
	*pp_list = temp;
	return XXX_OK;
}//x

// Länge der Liste
int xxx_List_Length(XXXList *p_list)
{
	XXXList *temp;
	int int_i =0;

	if(p_list == NULL)
		return int_i;

	int_i =1;
	temp = p_list;
	while(temp->next != NULL)
	{
		int_i++;
		temp = temp->next;
	}
	return int_i;
}//x

// Ausgabe der Liste
void xxx_List_Show(XXXList *p_list)
{
	XXXList *temp;
	int int_i =0;

	temp=p_list;
	printf("\n");
	while(temp != NULL)
	{
		printf("Liste[%d] = %c\n",int_i,(char)temp->data);
		temp=temp->next;
		int_i++;
	}
	printf("Length: %d\n",xxx_List_Length(p_list));
}//x