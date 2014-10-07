/*
 *
 *	calendar.c
 *
 * 	Program to generate a 1962 Extraordinary Form Roman Catholic Liturgical Calendar
 *	the specifics and information used to write this program have been take from
 *  the fisheaters website, http://www.fisheaters.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calendar.h"

day_t g_liturgicalday[366];

int g_daysinmonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int g_daycodetable[] = { 0, 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };

const int c_1d = 1;
const int c_2d = 2;
const int c_3d = 3;
const int c_4d = 4;
const int c_5d = 5;
const int c_6d = 6;
const int c_7d = 7;

const int c_1w = 7;
const int c_2w = 14;
const int c_3w = 21;
const int c_4w = 28;
const int c_5w = 35;
const int c_6w = 42;
const int c_7w = 49;
const int c_8w = 56;
const int c_9w = 63;
const int c_10w = 70;

const char *g_months[][2]=
{
	{ " ", " " },
	{ "January", "Dedicated to the Holy Name of Jesus" },
	{ "February", "Dedicated to the Holy Family" },
	{ "March", "Dedicated to St Joseph" } ,
	{ "April", "Dedicated to the Blessed Sacrament" },
	{ "May", "Dedicated to the Blessed Virgin Mary" },
	{ "June", "Dedicated to the Sacred Heart of Jesus" },
	{ "July", "Dedicated to the Precious Blood" },
	{ "August", "Dedicated to the Immaculate Heart of Mary" },
	{ "September", "Dedicated to the Seven Sorrows" },
	{ "October", "Dedicated to the Holy Rosary" },
	{ "November", "Dedicated to the Holy Souls in Purgatory" },
	{ "December", "Dedicated to the Immaculate Conception" },
};

const char *g_days[][2]=
{
	{ " ", " " },
	{ "Sunday", "Holy Trinity" },
	{ "Monday", "Holy Angels" },
	{ "Tuesday", "Apostles" },
	{ "Wednesday", "St Joseph" },
	{ "Thursday ", "Blessed Sacrament" },
	{ "Friday", "Christ's Passion" },
	{ "Saturday ", "Blessed Virgin" },
};

/*
 * returns the aggregate day in the year given a month
 */
int determinedayinyear(month_t month)
{
	int i;
	int dayinyear = 0;

	for (i = 1; i <= month; i++)
	{
		dayinyear += g_daysinmonth[i];
	}
	
	return (dayinyear); 
}

int determineprecedence(day_t* pday, int day, type_t type, class_t class)
{
	int updateday = 0;

	if (pday->day == 0)
	{
		updateday = 1;
	}
	else
	{
		if (class < pday->class)
		{
			updateday = 1;
			return (updateday);
		}

		if ((pday->type == sunday) && (class == class_i))
		{
			if (day == ImmaculateConception)
				updateday = 1;

			return (updateday);
		}

		if ((pday->type == sunday) && (pday->class == class_ii))
		{
			if ((type == feast) && (class == class_ii))
			{
				if (day < pday->day)
					updateday = 1;

				return (updateday);
			}
		}

		if ((type == feria) && (class == class_ii))
		{
			switch (day)
			{
				case AshWednesday:
				case HolyMonday:
				case HolyTuesday:
				case SpyWednesday:
				case MaundyThursday:
				case GoodFriday:
				case HolySaturday:
					updateday = 1;
					break;
			}
		}

		switch (day)
		{
			case Easter_Mon:
			case Easter_Tue:
			case Easter_Wed:
			case Easter_Thur:
			case Easter_Fri:
			case Pentecost_Mon:
			case Pentecost_Tue:
			case Whit_WedEmber:
			case Pentecost_Thur:
			case Whit_FriEmber:
			case Whit_SatEmber:
				updateday = 1;
				break;
		}
	}

	
	
	return (updateday);
}

/*
 * takes a month and day and sets the liturgical day and title
 */
void setmonthday(month_t month, int dayinmonth, int day, type_t type, class_t class, char* name)
{
	int dayinyear;

	dayinyear = determinedayinyear(month-1);
	dayinyear += dayinmonth - 1;

	if (determineprecedence(&g_liturgicalday[dayinyear], day, type, class))
	{
		g_liturgicalday[dayinyear].day = day;
		g_liturgicalday[dayinyear].type = type;
		g_liturgicalday[dayinyear].class = class;
		g_liturgicalday[dayinyear].name = name;
	}
}

/*
 * takes a month, day and offset and sets the liturgical day and title
 */
void setmonthdayoffset(month_t month, int dayinmonth, int offset, int day, type_t type, class_t class, char* name)
{
	int dayinyear;

	dayinyear = determinedayinyear(month-1);
	dayinyear += dayinmonth + offset - 1;

	if (determineprecedence(&g_liturgicalday[dayinyear], day, type, class))
	{
		g_liturgicalday[dayinyear].day = day;
		g_liturgicalday[dayinyear].type = type;
		g_liturgicalday[dayinyear].class = class;
		g_liturgicalday[dayinyear].name = name;
	}
}

/*
 * takes a month and day and sets the note for the liturgical day
 */
void setmonthdaynote(month_t month, int dayinmonth, char* note)
{
	int dayinyear;

	dayinyear = determinedayinyear(month-1);
	dayinyear += dayinmonth - 1;

	if (g_liturgicalday[dayinyear].note == 0)
		g_liturgicalday[dayinyear].note = note;
}

/*
 * takes a month, day and offset and sets the note 
 */
void setmonthdayoffsetnote(month_t month, int dayinmonth, int offset, int day, char* note)
{
	int dayinyear;

	dayinyear = determinedayinyear(month-1);
	dayinyear += dayinmonth + offset - 1;

	if (g_liturgicalday[dayinyear].note == 0)
		g_liturgicalday[dayinyear].note = note;
}

/*
 * takes a month, day and offset and sets the devotion; 
 */
void setmonthdayoffsetdevotion(month_t month, int dayinmonth, int offset, char* devotion)
{
	int dayinyear;

	dayinyear = determinedayinyear(month-1);
	dayinyear += dayinmonth + offset - 1;

	if (g_liturgicalday[dayinyear].devotion == 0)
		g_liturgicalday[dayinyear].devotion = devotion;
}
/*
 * given a day and year return days code Monday, Tuesday, Wednesday, etc
 * the daycodes are defined by dayofweek_t 
 */
dayofweek_t determinedaycode(int month, int day, int year)
{
	dayofweek_t daycode;	
	int d1, d2, d3;
	int code = 0;

	d1 = year / 4;
	d2 = year / 100;
	d3 = year / 400;
	code = year;
	code += d1;
	code -= d2;
	code += d3;
 	code += day;
	code += g_daycodetable[month];
	code -= 1;
	daycode = code % 7;
	
	return daycode;
}

season_t determineseason(int dayinyear)
{
	int i;
	season_t season = pentecost;

	for (i = 0; i <= dayinyear; i++)
	{
		int day = g_liturgicalday[i].day;

		if (day == Epiphany)
			season = epiphany;
		else if (day == AshWednesday)
			season = lent;
		else if (day == Easter)
			season = easter;
		else if (day == Pentecost_1)
			season = pentecost;
		else if (day == Advent_1)
			season = advent;
	}

	return(season);
}
/*
 * determines whether the specific liturgical day requires a fast
 */
int getfastandabstinence(dayofweek_t dayofweek, int dayofyear)
{
	int type = none;
	season_t season = determineseason(dayofyear);

	if (dayofweek == fri) 
	{
		if (season != lent)
			return (abstain);
		else 
			return (fast);
	}
	

	switch (g_liturgicalday[dayofyear].day)
	{
		case VigilPentecost:
		case VigilChristmas:
		case AshWednesday:
		case GoodFriday:
		case HolySaturday:
		case Lent_WedEmber:
		case Lent_FriEmber:
		case Lent_SatEmber:
		case Advent_WedEmber:
		case Advent_FriEmber:
		case Advent_SatEmber:
		case Whit_WedEmber:
		case Whit_FriEmber:
		case Whit_SatEmber:
		case Michaelmas_WedEmber:
		case Michaelmas_FriEmber:
		case Michaelmas_SatEmber:
			return (fast);
		break;
	}

	if (season == lent)
	{
		switch (dayofweek)
		{
			case sat:
				type = fast;
				break;
			case sun:
				type = none;
				break;
			default:
				type = partial;
				break;
		}

		if (g_liturgicalday[dayofyear].class == class_i)
		{
			type = none;
		}
	}

	return (type);
}

/*
 * determines the Rosary Mysteries for a given day, this is
 * straightforward for all days but Sunday. Sundays depend
 * on the liturgical season
 */
char* getrosarymysteries(month_t month, int day, int year, int dayofyear)
{
	char *mysteries = " ";
	int i;

	int daycode = determinedaycode(month, day, year);

	switch (daycode)
	{
		case sunday:
			mysteries = "Glorious";

			for (i = 0; i <= dayofyear; i++) 
			{
				if (g_liturgicalday[i].day == AshWednesday)
					mysteries = "Sorrowful";
				else if (g_liturgicalday[i].day == Easter)
					mysteries = "Glorious";	
				else if (g_liturgicalday[i].day == Advent_1)
					mysteries = "Joyful";
				else if (g_liturgicalday[i].day == Christmas)
					mysteries = "Glorious"; 
			}
			break;
		case mon:
		case thu:
			mysteries = "Joyful";
			break;
		case tue:
		case fri:
			mysteries = "Sorrowful";
			break;	
		case wed:
		case sat:
			mysteries = "Glorious";
			break;
	}

	return(mysteries);
}

/*
 * given a specific day return the liturgical day 
 */
liturgicalday_t determineliturgicalday(int dayinyear)
{
	return(g_liturgicalday[dayinyear].day);
}

/*
 * given a specific day returns the name
 */
char* getdayname(int dayinyear)
{
	char *feria = "Feria";

	if (g_liturgicalday[dayinyear].name == 0)
	{
		return (feria);
	}

	return(g_liturgicalday[dayinyear].name);
}

/*
 * given a specific day return the note
 */
char* getnote(int dayinyear)
{
	if (g_liturgicalday[dayinyear].note == 0)
	{
		return ((char*)0);
	}

	return(g_liturgicalday[dayinyear].note);
}

/*
 * given a specific day return the note
 */
char* getdevotion(int dayinyear)
{
	if (g_liturgicalday[dayinyear].devotion == 0)
	{
		return ((char*)0);
	}

	return(g_liturgicalday[dayinyear].devotion);
}

/*
 * given a specific day return the color for that day
 */
char* getseasoncolor(int dayinyear)
{
	int i;
	char *seasoncolor = SEASON_YELLOW;
	int goldendayfirst = determinedayinyear(november) + 17 - 1;
	int goldendaylast = determinedayinyear(november) + 23 - 1;

	if ((dayinyear >= goldendayfirst) && (dayinyear <= goldendaylast))
	{
		seasoncolor = SEASON_GOLD;
		return (seasoncolor);
	}

	if (g_liturgicalday[dayinyear].day == Advent_3 ||
	    g_liturgicalday[dayinyear].day == Lent_4)
	{
		seasoncolor = SEASON_ROSE;
		return (seasoncolor);
	}

	for (i = 0; i <= dayinyear; i++)
	{
		int day = g_liturgicalday[i].day;

		if (day == Epiphany)
			seasoncolor = SEASON_GREEN;
		else if (day == Septuagesima)
			seasoncolor = SEASON_PURPLE;
		else if (day == Easter)
			seasoncolor = SEASON_YELLOW;
		else if (day == Pentecost_1)
			seasoncolor = SEASON_GREEN;
		else if (day == Advent_1)
			seasoncolor = SEASON_PURPLE;
	}





	return(seasoncolor);
}

/*
 * determine the month and day for Easter
 */
void determineeaster(int year, month_t *month, int *day)
{
    int c = year / 100;
    int n = year - 19 * (year / 19);
    int k = (c - 17 ) / 25;

    int i = c - c / 4 - (c - k) / 3 + 19 * n + 15;
    i = i - 30 * ( i / 30);
    i = i - (i / 28) * (1 - (i / 28) * (29 / (i + 1)) * ((21 - n) / 11));

    int j = year + year / 4 + i + 2 - c + c / 4;
    j = j - 7 * ( j / 7);

    int l = i - j;

    *month = 3 + (l + 40) / 44;
    *day = l + 28 - 31 * ( *month / 4);
}

/*
 * set the days for pentecost
 */
void determinepentecost(month_t month, int day)
{
	int offset = c_8w + c_1w; /* offset of Trinity Sunday from Easter */
	int dayinyear = determinedayinyear(month-1) + day - 1; /* day in yeaar to Easter */
	int  nsunday = 14;
	int done = 0;

	setmonthdayoffset(month, day, c_7w-c_1d, VigilPentecost, vigil, class_i, "Vigil of Pentecost<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w, Pentecost_1, sunday, class_i, "Pentecost<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_1d, Pentecost_Mon, octave, class_i, "Pentecost Monday<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_2d, Pentecost_Tue, octave, class_i, "Pentecost Tuesday<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_3d, Whit_WedEmber, feria, class_i, "Ember Wednesday after Pentecost<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_4d, Pentecost_Thur, octave, class_i, "Pentecost Thursday<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_5d, Whit_FriEmber, feria, class_i, "Ember Friday after Pentecost<BR>1st Class - Red");
	setmonthdayoffset(month, day, c_7w + c_6d, Whit_SatEmber, feria, class_i, "Ember Saturday after Pentecost<BR>1st Class = Red");

	/* detrmine the Sundays after Pentecost */
	while (!done)
	{
		if (determineliturgicalday(dayinyear + offset + c_1w) == Advent_1)
		{
			setmonthdayoffset(month, day, offset, Pentecost_Last, sunday, class_ii, "Last Sunday after Pentecost<BR>2nd Class - Green");
			done = 1;
		}
		else
		{
			switch (nsunday / 7)
			{
				case 2: setmonthdayoffset(month, day, offset, Pentecost_2, sunday, class_ii, "Second Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 3: setmonthdayoffset(month, day, offset, Pentecost_3, sunday, class_ii, "Third Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 4: setmonthdayoffset(month, day, offset, Pentecost_4, sunday, class_ii, "Fourth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 5: setmonthdayoffset(month, day, offset, Pentecost_5, sunday, class_ii, "Fifth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 6: setmonthdayoffset(month, day, offset, Pentecost_6, sunday, class_ii, "Sixth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 7: setmonthdayoffset(month, day, offset, Pentecost_7, sunday, class_ii, "Seventh Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 8: setmonthdayoffset(month, day, offset, Pentecost_8, sunday, class_ii, "Eighth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 9: setmonthdayoffset(month, day, offset, Pentecost_9, sunday, class_ii, "Ninth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 10: setmonthdayoffset(month, day, offset, Pentecost_10, sunday, class_ii, "Tenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 11: setmonthdayoffset(month, day, offset, Pentecost_11, sunday, class_ii, "Eleventh Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 12: setmonthdayoffset(month, day, offset, Pentecost_12, sunday, class_ii, "Twelfth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 13: setmonthdayoffset(month, day, offset, Pentecost_13, sunday, class_ii, "Thirteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 14: setmonthdayoffset(month, day, offset, Pentecost_14, sunday, class_ii, "Fourteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 15: setmonthdayoffset(month, day, offset, Pentecost_15, sunday, class_ii, "Fifteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 16: setmonthdayoffset(month, day, offset, Pentecost_16, sunday, class_ii, "Sixteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 17: setmonthdayoffset(month, day, offset, Pentecost_17, sunday, class_ii, "Seventeenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 18: setmonthdayoffset(month, day, offset, Pentecost_18, sunday, class_ii, "Eighteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 19: setmonthdayoffset(month, day, offset, Pentecost_19, sunday, class_ii, "Nineteenth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 20: setmonthdayoffset(month, day, offset, Pentecost_20, sunday, class_ii, "Twentieth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 21: setmonthdayoffset(month, day, offset, Pentecost_21, sunday, class_ii, "Twenty-first Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 22: setmonthdayoffset(month, day, offset, Pentecost_22, sunday, class_ii, "Twenty-second Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 23: setmonthdayoffset(month, day, offset, Pentecost_23, sunday, class_ii, "Twenty-third Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 24: setmonthdayoffset(month, day, offset, Pentecost_24, sunday, class_ii, "Twenty-fourth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 25: setmonthdayoffset(month, day, offset, Pentecost_25, sunday, class_ii, "Twenty-fifth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 26: setmonthdayoffset(month, day, offset, Pentecost_26, sunday, class_ii, "Twenty-sixth Sunday after Pentecost<BR>2nd Class - Green"); break;
				case 27: setmonthdayoffset(month, day, offset, Pentecost_27, sunday, class_ii, "Twenty-seventh Sunday after Pentecost<BR>2nd Class - Green"); break;
			}

			offset += c_1w;
			nsunday += c_1w;
		}
	}
}

/*
 * set the dates for advent 
 */
void determineadvent(int year)
{
	dayofweek_t daycode = determinedaycode(december, 25, year);
	setmonthday(december, 25, Christmas, class_i, feast, "Christmas<BR>1st Class - White");
	setmonthdayoffset(december, 25 - daycode, -c_3w, Advent_1, sunday, class_i, "First Sunday in Advent<BR>1st Class - Violet");
	setmonthdayoffset(december, 25 - daycode, -c_2w, Advent_2, sunday, class_i, "Second Sunday in Advent<BR>1st Class - Violet");
	setmonthdayoffset(december, 25 - daycode, -c_1w, Advent_3, sunday, class_i, "Gaudete Sunday<BR>1st Class - Rose");
	setmonthdayoffset(december, 25 - daycode, 0, Advent_4, sunday, class_i, "Fourth Sunday in Advent<BR>1st Class - Violet");
	setmonthdayoffset(december, 25 - daycode, -c_4d, Advent_WedEmber, feria, class_ii, "Ember Wednesday in Advent<BR>2nd Class - Violet");
	setmonthdayoffset(december, 25 - daycode, -c_2d, Advent_FriEmber, feria, class_ii, "Ember Friday in Advent<BR>2nd Class - Violet");
	setmonthdayoffset(december, 25 - daycode, -c_1d, Advent_SatEmber, feria, class_ii, "Ember Saturday in Advent<BR>2nd Class - Violet");
	setmonthdayoffsetdevotion(december, 25, -(c_1w + c_1d), "<a href=\"http://www.fisheaters.com/customsadvent10.html\">First Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_7d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Second Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_6d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Third Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_5d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Fourth Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_4d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Fifth Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_3d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Sixth Golden Night</a>");
	setmonthdayoffsetdevotion(december, 25, -c_2d, "<a href=\"http://www.fisheaters.com/customsadvent10.html\">Seventh Golden Night</a>"); 
}

void determinechristtheking(int year)
{
	int i;

	for (i = 0; i < 7; i++)
	{
		if (determinedaycode(october, 31 - i, year) == sun)	
		{
			setmonthday(october, 31 - i, ChristtheKing, feast, class_i, "Christ the King<BR>1st Class - White");
		}
	}
}

/*
 * set the dates for epiphany
 */
void determineepiphany(int year)
{
	dayofweek_t daycode = determinedaycode(january, 6, year);

	setmonthday(january, 6, Epiphany, sunday, class_i,  "Epiphany of Our Lord Jesus Christ<BR>1st Class - White");
	setmonthdayoffset(january, 6, c_1w - daycode, Epiphany_1, sunday, class_ii, "Feast of the Holy Family<BR>2nd Class - White");
	setmonthdayoffset(january, 6, c_2w - daycode, Epiphany_2, sunday, class_ii, "Second Sunday of Epiphany<BR>2nd Class - Green");
	setmonthdayoffset(january, 6, c_3w - daycode, Epiphany_3, sunday, class_ii, "Third Sunday of Epiphany<BR>2nd Class - Green");
	setmonthdayoffset(january, 6, c_4w - daycode, Epiphany_4, sunday, class_ii, "Fourth Sunday of Epiphany<BR>2nd Class - Green");
	setmonthdayoffset(january, 6, c_5w - daycode, Epiphany_5, sunday, class_ii, "Fifth Sunday of Epiphany<BR>2nd Class - Green");
	setmonthdayoffset(january, 6, c_6w - daycode, Epiphany_6, sunday, class_ii, "Sixth Sunday of Epiphany<BR>2nd Class - Green");
}

/*
 * set ember and rogation days
 */
void determinerogationdays(int year, month_t month, int day)
{
	dayofweek_t daycode = determinedaycode(september, 14, year);
	int emberoffset;

	setmonthday(september, 14, ExaltationofHolyCross, feast, class_i, "Feast of the Exaltation of the Holy Cross<BR>2nd Class - Red");

	if (daycode == sat)
		emberoffset = 11;
	else if (daycode == mon)
		emberoffset = 9;
	else if (daycode == tue)
		emberoffset = 8;
	else if (daycode == wed)
		emberoffset = 7;
	else if (daycode == thu)
		emberoffset = 13;
	else if (daycode == fri)
		emberoffset = 12;
	else if (daycode == sun)
		emberoffset = 10;

	setmonthdayoffset(september, 14, emberoffset, Michaelmas_WedEmber, feria, class_ii, "Ember Wednesday<BR>2nd Class - Violet"); 	
	setmonthdayoffset(september, 14, emberoffset + c_2d, Michaelmas_FriEmber, feria, class_ii, "Ember Friday<BR>2nd Class - Violet");
	setmonthdayoffset(september, 14, emberoffset + c_3d, Michaelmas_SatEmber, feria, class_ii, "Ember Saturday<BR>2nd Class - Violet");
	setmonthdayoffsetnote(april, 25, 0, Rogation, "Rogation Day");
	setmonthdayoffsetnote(month, day, 39-c_3d, RogationMon, "Rogation Day");
	setmonthdayoffsetnote(month, day, 39-c_2d, RogationTue, "Rogation Day");
	setmonthdayoffsetnote(month, day, 39-c_1d, RogationWed, "Rogation Day");

}

/*
 * determine the year is a leap year and set the days in february as needed
 */
int determineleapyear(int year)
{
	if((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
	{
		g_daysinmonth[2] = 29;
		g_daycodetable[1] = 6;
		g_daycodetable[2] = 2;
		return 1;
	}
	else
	{
		g_daysinmonth[2] = 28;
		g_daycodetable[1] = 0;
		g_daycodetable[2] = 3;
		return 0;
	}
}

/*
 * generate an html calendar
 */
void calendarhtml(int year, dayofweek_t daycode)
{
	month_t month;
	int day;
	int i;
	int daycounter = 0;

	printf("<HTML><HTML><TITLE>%d - EF Calendar</TITLE></HEAD><BR><BR>\n", year);
	for (month = january; month <= december; month++ )
	{
		printf("<CENTER>");
		printf("<TABLE BORDER=3 STYLE=\"BACKGROUND-COLOR:LIGHTGRAY\" BORDERCOLOR=\"BLACK\" CELLSPACING=3 CELLPADDING=3 HEIGHT=700 WIDTH=950>\n");
		printf("<TR>\n");
		printf("<ALIGN=center>\n");
		printf("<FONT SIZE=6><B>%s - %d</B><BR></FONT>\n", g_months[month][0], year);
		printf("<FONT SIZE=2><I>%s</I></FONT>\n", g_months[month][1]);
		printf("</CENTER>\n");
		printf("</TR>\n");
		printf("<BR><BR>\n");

		/* print day headers */
		printf("<TR>\n");	
		for (i = 1; i <= 7; i++)
		{
			printf("<TD ALIGN=center HEIGHT=50 WIDTH=125>\n");
			printf("<FONT SIZE=4><B>%s</B><BR></FONT>\n", g_days[i][0]);
			printf("<FONT SIZE=2><I>%s</I></FONT>\n", g_days[i][1]);
			printf("</TD>\n");
		}
		printf("</TR>\n");


		printf("<TR>\n");
		// Correct the position for the first date
		for ( day = 1; day <= daycode; day++ )
		{
			printf("<TD BGCOLOR=\"F5F5F5\"><BR></TD>\n");
		}
		
		// Print all the dates for one month
		for ( day = 1; day <= g_daysinmonth[month]; day++ )
		{
			dayofweek_t dayofweek = (day + daycode) % 7;

			printf("<TD HEIGHT=125 WIDTH=125 BGCOLOR=\"%s\" VALIGN=TOP ALIGN=LEFT><FONT SIZE=6><B>%d</B>\n", 
					getseasoncolor(daycounter), day);


			printf("<FONT SIZE=2><CENTER>%s</CENTER></FONT>\n", getdayname(daycounter));
			if (getnote(daycounter) != 0)
			{
				printf("<CENTER><FONT SIZE=1><CENTER>%s</CENTER></FONT></CENTER>\n", getnote(daycounter));
			}

			printf("<CENTER><FONT SIZE=1><a href=\"http://www.fisheaters.com/rosary.html\">%s</a></FONT></CENTER>\n", 
					getrosarymysteries(month, day, year, daycounter));	

			if (getdevotion(daycounter) != 0)
			{
				printf("<CENTER><FONT SIZE=0 VALIGN=TOP ALIGN=RIGHT>%s</FONT></CENTER>\n", getdevotion(daycounter)); 
			}

			switch (getfastandabstinence(determinedaycode(month, day, year), daycounter))
			{
				case abstain:
					printf("<CENTER><A HREF=\"http://www.fisheaters.com/fasting.html\"><IMG SRC=\"abstinencefish.gif\" HEIGHT=\"16\" WIDTH=\"50\"></A></CENTER>\n");
					break;
				case fast:
					printf("<CENTER><A HREF=\"http://www.fisheaters.com/fasting.html\"><IMG SRC=\"abstinencefish.gif\" HEIGHT=\"16\" WIDTH=\"50\"</A><FONT SIZE=1>FAST</FONT></CENTER>\n");
					break;
				case partial:
					printf("<CENTER><A HREF=\"http://www.fisheaters.com/fasting.html\"><IMG SRC=\"partialfish.gif\" HEIGHT=\"16\" WIDTH=\"25\"></A></CENTER>\n");
					break;
			}

			printf("</TD>\n");
			daycounter++;

			// Is day before Sat? Else start next line Sun.
			if ( dayofweek == 0 ) 
			{
				printf("</TR>\n" );
			}
		}

		// Set position for next month
		daycode = ( daycode + g_daysinmonth[month] ) % 7;

		// Correct the position for the first date
		if (daycode > 0)
		{
			for (day = daycode; day <= 6; day++ )
			{
				printf("<TD BGCOLOR=\"F5F5F5\"><BR></TD>\n");
			}
		}	

		printf("</CENTER>");
		printf("</TABLE>\n");
		printf("<BR><BR><BR><BR><BR>\n");
	}

	printf("</html>\n");
}

/*
 * set the sanctoral cycle
 */
void determinesanctoralcycle(int year)
{
	int day = 0;
	dayofweek_t daycode;
	
	setmonthday(january, 1, OctaveNativity, feast, class_i, "Feast of the Circumcision and Mary, Mother of God<BR>1st Class - White");
	setmonthdaynote(january, 1, "Octave Day of the Nativity of our Lord");
	do 
	{
		day++;
		daycode = determinedaycode(january, day, year);
	} while (daycode != sunday);
	if (day > 5)
		day = 2;

	/* January */
	setmonthday(january, day, MostHolyName, feast, class_ii, "Most Holy Name of Jesus<BR>2nd Class - White"); 
	setmonthdaynote(january, 11, "Comm. St Hyginus, PM");
	setmonthday(january, 13, BaptismLord, feast, class_ii, "Baptism of our Lord<BR>2nd Class - White");
	setmonthday(january, 14, StHillary, feast, class_iii, "St Hillary, BCD<BR>3rd Class - White");
	setmonthdaynote(january, 14, "Comm. St Felix, PrM");
	setmonthday(january, 15, StPaulHermit, feast, class_iii, "St Paul First Hermit<BR>3rd Class - White");
	setmonthdaynote(january, 15, "Comm. St Felix, PrM<BR>3rd Class - White");
	setmonthday(january, 16, StMarcellusI, feast, class_iii, "St Marcellis I<BR>3rd Class - Red");
	setmonthday(january, 17, StAntony, feast, class_iii, "St Antony Ab<BR>3rd Class - White");
	setmonthdaynote(january, 18, "Comm. St Prisca, VM");
	setmonthday(january, 20, SSFabianSebastion, feast, class_iii, "SS Fabian, PM and Sebastion, M<BR>3rd Class - Red");
	setmonthday(january, 21, StAgnes, feast, class_iii, "St Agnes, VM<BR>3rd Class - Red");
	setmonthday(january, 22, SSVincentAnastasius, feast, class_iii, "SS Vincent and Anastasius, MM<BR>3rd Class - Red");
	setmonthday(january, 23, StRaymondPenafort, feast, class_iii, "St Raymond of Penafort, C<BR>3rd Class - White");
	setmonthdaynote(january, 23, "Comm. St Emerentiana, VM");
	setmonthday(january, 24, StTimothy, feast, class_iii, "St Timothy, BM<BR>3rd Class - Red");
	setmonthday(january, 25, ConversionStPaul, feast, class_iii, "Conversion of St Paul, Ap<BR>3rd Class - White");
	setmonthdaynote(january, 25, "Comm. St Peter, Ap");
	setmonthday(january, 27, StJohnChrysostom, feast, class_iii, "St John Chrysostom, BCD<BR>3rd Class - White");
	setmonthday(january, 28, StPeterNolasco, feast, class_iii, "St Peter Nolasco, C<BR>3rd Class - White");
	setmonthdaynote(january, 28, "Comm. St Agnes, VM");
	setmonthday(january, 29, StFrancisdeSales, feast, class_iii, "St Francis de Sales, BCD<BR>3rd Class - White");
	setmonthday(january, 30, StMartina, feast, class_iii, "St Martina, VM<BR>3rd Class - Red");
	setmonthday(january, 31, StJohnBosco, feast, class_iii, "St John Bosco, C<BR>3rd Class - White");

	/* February */
	setmonthday(february, 1, StIgnatiusofAntioch, feast, class_iii, "St Ignatius of Antioch<BR>3rd Class - Red");
	setmonthday(february, 2, PurificationofVirgin, feast, class_ii, "Purification of the Blessed Virgin Mary<BR>2nd Class - White");
	setmonthdaynote(february, 3, "Comm of St Blaise, BM"); 
	setmonthday(february, 4, StAndrewCorsini, feast, class_iii, "St Andrew Corsini, BC<BR>3rd Class - White"); 
	setmonthday(february, 5, StAgatha, feast, class_iii, "St Agatha, VN<BR>3rd Class - Red");
	setmonthday(february, 6, StTitus, feast, class_iii, "St Titus, BC<BR>3rd Class - White");
	setmonthday(february, 7, StRomuald, feast, class_iii, "St Romuald, Ab<BR>3rd Class - White");
	setmonthday(february, 8, StJohnofMatha, feast, class_iii, "St Jon of Matha, C<BR>3rd Class - White");
	setmonthday(february, 9, StCyrilofAlexandria, feast, class_iii, "St Cyril of Alexandria, BCD<BR>3rd Class - White");
	setmonthdaynote(february, 9, "Comm of St Apollonia, VM");
	setmonthday(february, 10, StScholastica, feast, class_iii, "St Scholastica, V<BR>3rd Class - White");
	setmonthday(february, 11, OurLadyofLourdes, feast, class_iii, "Apparation of Our Lady of Lourdes<BR>3rd Class - White");
	setmonthday(february, 12, HolyFoundersServites, feast, class_iii, "Seven Holy Founders of the Servites, CC<BR>3rd Class - White");
	setmonthdaynote(february, 14, "Comm St Valentine, PrM");
	setmonthdaynote(february, 15, "Comm SS Faustinius and Jovita, MM");
	setmonthdaynote(february, 18, "Comm St Simeon, BM<BR>St Bernadette, V");
	setmonthday(february, 22, ChairStPeter, feast, class_ii, "Chair of St Peter, Ap<BR>2nd Class - White");
	setmonthdaynote(february, 22, "Comm St Paul, Ap");
	setmonthday(february, 23, StPeterDamian, feast, class_iii, "St Peter Damian, BCD<BR>3rd Class - White");
	setmonthday(february, 24, StMatthias, feast, class_ii, "St Mathias<BR>2nd Class - Red");
	setmonthday(february, 27, StGabrielSorrows, feast, class_iii, "St Gabriel of Sorrows<BR>3rd Class - White");

	/* March */
	setmonthday(march, 4, StCasimer, feast, class_iii, "St Casimir of Lithuania, C<BR>3rd Class - White");
	setmonthdaynote(march, 4, "Comm St Lucis I, PM");
	setmonthday(march, 6, SSPerpetuaFelicitas, feast, class_iii, "SS Perpetua and Felicitas<BR>3rd Class - Red");
	setmonthday(march, 7, StThomasAquinas, feast, class_iii, "St Thomas Aquinas, CD<BR>3rd Class - White");
	setmonthday(march, 8, StJohnofGod, feast, class_iii, "St John of God<BR>3rd Class - white");
	setmonthday(march, 9, StFrancisofRome, feast, class_iii, "St Francis of Rome, W<BR>3rd Class - White");
	setmonthday(march, 10, FortyMartyrsofSebaste, feast, class_iii, "Forty Martyrs of Sebaste<BR>3rd Class - Red");
	setmonthday(march, 12, GregorytheGreat, feast, class_iii, "St Gregory the Great, PCD<BR>3rd Class - White");
	setmonthday(march, 17, StPatrick, feast, class_iii, "St Patrick, BC<BR>3rd Class - white");
	setmonthday(march, 18, StCyril, feast, class_iii, "St Cyril, BCD<BR>3rd Class - White");
	setmonthday(march, 19, StJoseph, feast, class_i, "St Joseph, Spouse of the Blessed Virgin Mary<BR>1st Class - White");
	setmonthday(march, 21, StBenedict, feast, class_iii, "St Benedict, Ab<BR>3rd Class - White");
	setmonthday(march, 24, StGabrielArchangel, feast, class_iii, "St Gabriel the Archangel<BR>3rd Class - White"); 
	setmonthday(march, 25, AnnunciationofOurLady, feast, class_i, "Annunciation of the Blessed Virgin Mary<BR>1st Class - White");
	setmonthday(march, 27, StJohnDamascene, feast, class_iii, "St John Damascene, CD<BR>3rd Class - White");
	setmonthday(march, 29, StJohnCapistran, feast, class_iii, "St John Capistran, C<BR>3rd Class - White"); 

	/* April */
	setmonthday(april, 1, StFrancisofPaul, feast, class_iii, "St Francis of Paul, C<BR>3rd Class - White");
	setmonthday(april, 4, StIsidore, feast, class_iii, "St Isidore, BCD<BR>3rd Class - White");
	setmonthday(april, 5, StVincentFerror, feast, class_iii, "St Vincent Ferror, C<BR>3rd Class - White"); 
	setmonthday(april, 11, StLeoI, feast, class_iii, "St Leo I, PCD<BR>3rd Class - White");
	setmonthday(april, 13, StHermenegild, feast, class_iii, "St Hermenegild, M<BR>3rd Class - Red");
	setmonthday(april, 14, StJustin, feast, class_iii, "St Justin, M<BR>3rd Class - Red");
	setmonthdaynote(april, 14, "Comm of SS Tiburtius, Valerian and Maximus, MM");
	setmonthdaynote(april, 17, "Comm of Anicetus, PM");
	setmonthday(april, 21, StAnselm, feast, class_iii, "St Anselm, BCD<BR>3rd Class - White");
	setmonthday(april, 22, SSSoterCaius, feast, class_iii, "SS Soter and Caius, PM<BR>3rd Class - White");
	setmonthdaynote(april, 23, "Comm of St George, M");
	setmonthday(april, 24, StFidelis, feast, class_iii, "St Fidelis of Sigmaringen, M<BR>3rd Class Red");
	setmonthday(april, 25, StMark, feast, class_ii, "St Mark, E<BR>2d Class - Red");
	setmonthday(april, 26, SSCletusMarcellinus, feast, class_iii, "SS Cletus and Marcellinus, PM<BR>3rd Class - Red");
	setmonthday(april, 27, StPeterCanisius, feast, class_iii, "St Peter Canisus, CD<BR>3rd Class - White");
	setmonthday(april, 28, StPauloftheCross, feast, class_iii, "St Paul of the Cross, CF<BR>3rd Class - White");
	setmonthday(april, 29, StPeterofVerona, feast, class_iii, "St Peter of Verona, M<BR>3rd Class - Red");
	setmonthday(april, 30, StCatherineofSiena, feast, class_iii, "St Catherine of Siena, V<BR>3rd Class - White");   

	/* May */
	setmonthday(may, 1, StJosephtheWorker, feast, class_i, "St Joseph the Worker, Spouse of the Blessed Virgin Mary");
	setmonthday(may, 2, StAthanasius, feast, class_iii, "St Athanius, BCD<BR>3rd Class - White");
	setmonthdaynote(may, 3, "Comm of SS Alexandr and his Comps Eventius and Theodulus, MM");
	setmonthday(may, 4, StMonica, feast, class_iii, "St Monica, W<BR>3rd Class - White");
	setmonthday(may, 5, StPiusV, feast, class_iii, "St Pius V, PC<BR>3rd Class - White");
	setmonthday(may, 7, StStanislaus, feast, class_iii, "St Stanisluas, BM<BR>3rd Class - Red");
	setmonthday(may, 9, StGregoryNazianzen, feast, class_iii, "St Gregory Nazianzen, BCD<BR>3rd Class - White");
	setmonthday(may, 10, StAntoninius, feast, class_iii, "St Antoninus, BC<BR>3rd Class - White");
	setmonthdaynote(may, 10, "Comm of SS Gordian and Epimachus");
	setmonthday(may, 11, SSPhilipJames, feast, class_ii, "SS Philip and James, Ap<BR>2nd Class - Red");
	setmonthday(may, 12, SSNerusAchilleus, feast, class_iii, "SS Nerus, Achilleus, MM. St Domitilla, VM. St Pancras, M<BR>3rd Class - Red");
	setmonthday(may, 13, StRobertBellarmine, feast, class_iii, "St Robert Bellarmine, BCD<BR>3rd Class - White");
	setmonthdaynote(may, 14, "Comm St Boniface, M");
	setmonthday(may, 15, StJohnBaptiste, feast, class_iii, "St John Baptiste de la Salle, C<BR>3rd Class - White");
	setmonthday(may, 16, StUbaldus, feast, class_iii, "St Ubaldus, B<BR>3rd Class - White");
	setmonthday(may, 17, StPascalBaylon, feast, class_iii, "St Pascal Baylon, C<BR>3rd Class - White");
	setmonthday(may, 18, StVenantius, feast, class_iii, "St Venaantius of Camernino, M<BR>3rd Class - Red");
	setmonthday(may, 19, StPeterCelestine, feast, class_iii, "St Peter Celestine, PC<BR>3rd Class - White");
	setmonthday(may, 20, StBernadineofSiena, feast, class_iii, "St Bernadine of Siena, C<BR>3rd Class - White");
	setmonthday(may, 25, StGregory, feast, class_iii, "St Gregory VII, PC<BR>3rd Class - White");
	setmonthday(may, 26, StPhilipNeri, feast, class_iii, "St Philip Neri, C<BR>3rd Class - White");
	setmonthdaynote(may, 26, "Comm of St Eleutheris, PM");
	setmonthday(may, 27, StBedetheVenerable, feast, class_iii, "St Bede the Venerable, CD<BR>3rd Class - White");
	setmonthday(may, 28, StAugustineofCanterbury, feast, class_iii, "St Augustine, B<BR>3rd Class - White");
	setmonthday(may, 29, StMaryMagdelenofPazzi, feast, class_iii, "St Mary Magdelene of Pazzi, V<BR>3rd Class - White");
	setmonthdaynote(may, 30, "Comm of St Felix I, PM");
	setmonthday(may, 31, OurLadyVirginandQueen, feast, class_ii, "Our Lady Virgin and Queen<BR>2nd Class - White");
	setmonthdaynote(may, 31, "Comm of St Petronilla, V"); 

	/* June */
	setmonthday(june, 1, StAngelaMerici, feast, class_iii, "St Angela Merici, V<BR>3rd Class - White");
	setmonthdaynote(june, 2, "SS Marcellinus and Peter, M. St Erasimus, BM");
	setmonthday(june, 4, StFrancisCaracciolo, feast, class_iii, "St Francis Caracciolo, C<BR>3rd Class - White");
	setmonthday(june, 5, StBoniface, feast, class_iii, "St Boniface BM<BR>3rd Class - White");
	setmonthday(june, 6, StNorbert, feast, class_iii, "St Norbert, BC<BR>3rd Clas - White");
	setmonthdaynote(june, 9, "Comm of SS Primus and Felician, MM");
	setmonthday(june, 10, StMargaret, feast, class_iii, "St Margaret Queen of Scotland, W<BR>3rd Class - White");
	setmonthday(june, 11, StBarnabas, feast, class_ii, "St Barnabas, Ap<BR>2nd Class - Red"); 
	setmonthday(june, 12, StJohnofSanFacundo, feast, class_iii, "St John of Facundo, C<BR>3rd Class - White");
	setmonthday(june, 13, StAnthonyofPadua, feast, class_iii, "St Anthony of Padua, CD<BR>3rd Class - White");
	setmonthday(june, 14, StBasiltheGreat, feast, class_iii, "St Basil the Great, BCD<BR>3rd Class - White");
	setmonthdaynote(june, 15, "Comm of SS Vitus or Guy, Modestus and Crescentia, MM");
	setmonthday(june, 17, StGregoryBarbarigo, feast, class_iii, "St Gregory Barbarigo, BC<BR>3rd Class - White");
	setmonthday(june, 18, StEphram, feast, class_iii, "St Ephram, DCD<BR>3rd Class - White");
	setmonthdaynote(june, 18, "Comm of SS Mark and Marcellian");
	setmonthday(june, 19, StJulianaFalconieri, feast, class_iii, "St Juliana Falconieri, V<BR>3rd Class - White");
	setmonthdaynote(june, 19, "Comm of SS Gervase and Protase, MM");
	setmonthdaynote(june, 20, "Comm of St Silverius");
	setmonthday(june, 21, StAloyiusGonzaga, feast, class_iii, "St Alonyius Gonzaga, C<BR>3rd Class - White");
	setmonthday(june, 22, StPaulinus, feast, class_iii, "St Paulinus, B<BR>3rd Class - White");
	setmonthday(june, 24, NativityofStJohnBaptist, feast, class_i, "Nativity of St John the Baptist<BR>1st Class - White");
	setmonthday(june, 25, StWilliam, feast, class_iii, "St William, Ab<BR>3rd Class - White");
	setmonthday(june, 26, SSJohnandPaul, feast, class_iii, "SS John and Paul, MM<BR>3rd Class - White");
	setmonthday(june, 29, SSPeterandPaul, feast, class_i, "SS Peter and Paul, Ap<BR>1st Class - Read");
	setmonthday(june, 30, CommStPaul, feast, class_iii, "Comm of St Paul, Ap<BR>3rd Class - Red");

	/* July */
	setmonthday(july, 1, MostPreciousBlood, feast, class_i, "The Feast of the Most Precious Blood of Our Lord Jesus Christ<BR>1st Class - White");
	setmonthday(july, 2, VisitationofOurLady, feast, class_ii, "The Visitation of the Blessed Virgin Mary<BR>2nd Class - White");
	setmonthdaynote(july, 2, "Comm of SS Processus and Martinian, M");
	setmonthday(july, 3, StLeoII, feast, class_iii, "St. Leo II, PC<BR>3rd Class - White");
	setmonthday(july, 5, StAnthonyMariaZaccaria, feast, class_iii, "St. Anthony Mary Zaccaria, C<BR>3rd Class - White");
	setmonthday(july, 7, SSCyrilMethodius, feast, class_iii, "SS Cyril and Methodius, BC<BR>3rd Class - White");
	setmonthday(july, 8, StElizabeth, feast, class_iii, "St. Elizabeth, W<BR>3rd Class - White");
	setmonthday(july, 10, SevenHolyBrothers, feast, class_iii, "The Seven Holy Brothers, Martyrs, and Sts. Rufina and Secunda, VM<BR>3rd Class - Red");
	setmonthdaynote(july, 11, "Comm of St Pius I, PM");
	setmonthday(july, 12, StJohnGualbert, feast, class_iii, "St John Gualbert, Ab<BR>3rd Class - White");
	setmonthdaynote(july, 12, "Comm of SS Nabor and Felix, M");
	setmonthday(july, 13, StAnacletus,  feast, class_iii, "St. Anacletus, PM<BR>3rd Class - Red");
	setmonthday(july, 14, StBonaventure, feast, class_iii, "St Bonaventure, BCD<BR>3rd Class - White");
	setmonthday(july, 15, StHenry, feast, class_iii, "St Henry, EC<BR>3rd Class - White");
	setmonthdaynote(july, 16, "Comm of Our Lady of Mount Carmel");
	setmonthdaynote(july, 17, "Comm of St Alexius");
	setmonthday(july, 18, StCamillusdeLellis, feast, class_iii, "St Camillus de Lellis, C<BR>3rd Class - White");
	setmonthdaynote(july, 18, "Comm of St Symphorosa and Her Sevens Sons, M");
	setmonthday(july, 19, StVincentdePaul, feast, class_iii, "St Vincent de Paul, C<BR>3rd Class - White");
	setmonthday(july, 20, StJeromeEmilian, feast, class_iii, "St Jerome Emilian, C<BR>3rd Class - White");
	setmonthdaynote(july, 20, "Comm of St Margaret, VM");
	setmonthday(july, 21, StLawrenceBrindisi, feast, class_iii, "St Lawrence Brindisi, CD<BR>3rd Class - White");
	setmonthdaynote(july, 21, "St. Praxedes, V");
	setmonthday(july, 22, StMaryMagdalen, feast, class_iii, "St Mary Magdalen, P<BR>3rd Class - White");
	setmonthday(july, 23, StApollinaris, feast, class_iii, "St Apollinaris, BM<BR>3rd Class - Red");
	setmonthdaynote(july, 23, "Comm of St Liborius, B");
	setmonthdaynote(july, 24, "Comm of St Christina, VM");
	setmonthday(july, 25, StJamestheGreater, feast, class_ii, "St James the Greater, Ap<BR>2nd Class - Red");
	setmonthday(july, 26, StAnne, feast, class_ii, "St Anne, Mother of the Blessed Virgin Mary<BR>2nd Class - White");
	setmonthdaynote(july, 27, "Comm of St Pantaleon");
	setmonthday(july, 28, StNazariusCelsus, feast, class_iii, "SS Nazarius, Celsus, M. St Victor I, PM, St Innocent I, PM<BR>3rd Class - Red");
	setmonthday(july, 29, StMartha, feast, class_iii, "St Martha, V<BR>3rd Class - White");
	setmonthdaynote(july, 29, "Comm of St Felix II, PM. SS Simplicius, Faustinas and Beatrice, M");
	setmonthdaynote(july, 30, "Comm of SS Addon and Sennen, M");
	setmonthday(july, 31, StIgnatiusofLoyola, feast, class_iii, "St Ignatius Loyola, C<BR>3rd Class - White");

	/* August */
	setmonthdaynote(august, 1, "Comm of the Seven Machabees, M");
	setmonthday(august, 2, StAlphonsusLigouri, feast, class_iii, "St Alphonsus Ligouri, BCD<BR>3rd Class - White");
	setmonthday(august, 4, StDominic, feast, class_iii, "St Dominic, C<BR>3rd Class - White");
	setmonthday(august, 5, OurLadyoftheSnow, feast, class_iii, "Dedication of Our Lady of the Snow<BR>3rd Class - White");
	setmonthday(august, 6, Transfiguration, feast, class_ii, "Transfiguration of Our Lord<BR>2nd Class - White");
	setmonthdaynote(august, 6, "Comm of SS Sixtus II, P. Fellicissmus and Agapiyus, M");
	setmonthday(august, 7, StCajetan, feast, class_iii, "St Cajetan, C<BR>3rd Class - White");
	setmonthdaynote(august, 7, "Comm of St Donatus, BM");
	setmonthday(august, 8, StJohnVianney, feast, class_iii, "St John Mary Vianney, C<BR>3rd Class - White");
	setmonthdaynote(august, 8, "Comm of SS Cyriacus, Largus, Smaragdus, M");
	setmonthday(august, 9, VigilofStLawrence, vigil, class_iii, "Vigil of St Lawrence<BR>3rd Class - White"); 
	setmonthdaynote(august, 9, "Comm of St Romanus, M"); 
	setmonthday(august, 10, StLawrence, feast, class_ii, "St Lawrence, M<BR>2nd Class - Red");
	setmonthdaynote(august, 11, "Comm of SS Tiburtius, M. and Susanna, VM");
	setmonthday(august, 12, StClare, feast, class_iii, "St Clare, V<BR>3rd Class - White");
	setmonthdaynote(august, 13, "Comm of SS Hippolytus and Cassian, M");
	setmonthday(august, 14, VigiloftheAssumption, vigil, class_ii, "Vigil of the Assumption<BR>2nd Class - White");
	setmonthday(august, 15, Assumption, feast, class_i, "Assumption of the Blessed Virgina Mary<BR>1st Class - White");
	setmonthday(august, 16, StJoachim, feast, class_ii, "St Joachim, Father of Our Blessed Lady<BR>2nd Class - White");
	setmonthday(august, 17, StHyacinth, feast, class_iii, "St Hyacinth, C<BR>3rd Class - White");
	setmonthdaynote(august, 18, "Comm of St Agapiyus, M");
	setmonthday(august, 19, StJohnEudes, feast, class_iii, "St John Eudes, C<BR>3rd Class - White");
	setmonthday(august, 20, StBernard, feast, class_iii, "St Bernard, AbCD<BR>3rd Class - White");
	setmonthday(august, 21, StJaneFrancesFremoitdeChantel, feast, class_iii, "St Jane Frances Fremoit de Chantel, W<BR>3rd Class - White");
	setmonthday(august, 22, ImmaculateHeartofMary, feast, class_ii, "Immaculate Heart of Mary<BR>2nd Class - White");
	setmonthdaynote(august, 22, "Comm of SS Timotheus, M. Hippolytus, BM. Symphorianus, M");
	setmonthday(august, 23, StPhilipBenizi, feast, class_iii, "St Philip Benizi, C<BR>3rd Class - White");
	setmonthday(august, 24, StBartholomew, feast, class_ii, "St Bartholomew, Ap<BR>2nd Class - White");
	setmonthday(august, 25, StLouisIX, feast, class_iii, "St Louis IX, King of France<BR>3rd Class - White");
	setmonthdaynote(august, 26, "Comm of St Zephyrinus, PM<BR>3rd Class - Red");
	setmonthday(august, 27, StJosephCalasanctius, feast, class_iii, "St Joseph Calasanctius, C<BR>3rd Class - White");
	setmonthday(august, 28, StAugustine, feast, class_iii, "St Augustine, Bishop of Hippo, CD<BR>3rd Class - White");
	setmonthday(august, 29, BeheadingofStJohn, feast, class_iii, "Beheading of St John the Baptist<BR>3rd Class - Red");
	setmonthday(august, 30, StRoseofLima, feast, class_iii, "St Rose of Lima, V<BR>3rd Class - White");
	setmonthdaynote(august, 30, "Comm of SS Felix and Adauctus, MM");
	setmonthday(august, 31, StRaymondNonnatus, feast, class_iii, "St Raymond of Nonnatus, C<BR>3rd Class - White");

	/* September */
	setmonthdaynote(september, 1, "Comm of St Giles, Ab. The Twelve Holy Brothers, M");
	setmonthday(september, 2, StStephenKingofHungary, feast, class_iii, "St Stephen, King of Hungary<BR>3rd Class - White");
	setmonthday(september, 3, StPiusX, feast, class_iii, "St Pius X, PC<BR>3rd Class - White");
	setmonthday(september, 5, StLawrenceJustinian, feast, class_iii, "St Lawrence Justinian, BC<BR>3rd Class - White");
	setmonthday(september, 8, NativityoftheBlessedVirginMary, feast, class_ii, "Nativity of the Blessed Virgin Mary<BR>2nd Class - White");
	setmonthdaynote(september, 8, "Comm of St Adrian, M");
	setmonthdaynote(september, 9, "Comm of St Gorgonius, M");
	setmonthday(september, 10, StNicholasofTolentino, feast, class_iii, "St Nicholas of Tolentino, C<BR>3rd Class - White");
	setmonthdaynote(september, 11, "Comm of St Protus and Hyacinth, MM");
	setmonthday(september, 12, MostHolyNameofMary, feast, class_iii, "Most Holy Name of Mary<BR>3rd Class - White");
	setmonthday(september, 14, ExaltationoftheHolyCross, feast, class_ii, "Exaltation of the Holy Cross<BR>2nd Class - White");
	setmonthday(september, 15, SevenSorrows, feast, class_ii, "Seven Sorrows of our Blessed Lady<BR>2nd Class - White");
	setmonthdaynote(september, 15, "Comm of St Nicodemes, M");
	setmonthday(september, 16, StCornelius, feast, class_iii, "St Cornelius, PM<BR>3rd Class - Red");
	setmonthdaynote(september, 16, "Comm of St Euphemia and Companions, MM");
	setmonthdaynote(september, 17, "Comm of the Stigmata of St Francis, C");
	setmonthday(september, 18, StJosephofCupertino, feast, class_iii, "St Joseph of Cupertino, C<BR>3rd Class - White");
	setmonthday(september, 19, StJanarius, feast, class_iii, "St Janarius, BM and Companions<BR>3rd Class - Red");
	setmonthdaynote(september, 20, "Comm of St Eustace and Companions, MM");
	setmonthday(september, 21, StMatthew, feast, class_ii, "St Matthew, ApE<BR>2nd Class - Red");
	setmonthday(september, 22, StThomasofVillanova, feast, class_iii, "St Thomas of Villanova, BC<BR>3rd Class - White");
	setmonthdaynote(september, 22, "Comm of St Maurice and Companions, MM");
	setmonthday(september, 23, StLinus, feast, class_iii, "St Linus, PM<BR>3rd Class - Red");
	setmonthdaynote(september, 23, "Comm of St Thecla, VM");
	setmonthdaynote(september, 24, "Comm of Our Lady of Ransom");
	setmonthdaynote(september, 26, "Comm of SS Cyprian and Justina, V, MM");
	setmonthday(september, 27, SSCosmasDamian, feast, class_iii, "SS Cosmas and Damian, MM<BR>3rd Class - Red");
	setmonthday(september, 28, StWencelaus, feast, class_iii, "St Wencelaus, Duke of Bohemia, M<BR>3rd Class - Red");
	setmonthday(september, 30, StMichael, feast, class_i, "Dedication of St Michael the Archangel<BR>1st Class - White");
	setmonthday(september, 31, StJerome, feast, class_iii, "St Jerome, PrCD<BR>3rd Class - White");

	/* October */
	setmonthdaynote(october, 1, "Comm of St Remigus, B");
	setmonthday(october, 2, HolyGuardianAngels, feast, class_iii, "Holy Guardian Angels<BR>3rd Class - White");
	setmonthday(october, 3, StTeresaoftheChild, feast, class_iii, "St Teresa of the Child, V<BR>3rd Class - White");
	setmonthday(october, 4, StFrancisofAssisi, feast, class_iii, "St Francis of Assisi, C<BR>3rd Class - White");
	setmonthdaynote(october, 5, "Comm of St Placid and Companions, MM");
	setmonthday(october, 6, StBruno, feast, class_iii, "St Bruno, C<BR>3rd Class - White");
	setmonthday(october, 7, OurLadyoftheRosary, feast, class_ii, "Our Lady of the Rosary<BR>2nd Class - White");
	setmonthdaynote(october, 7, "Comm of St Mark, PC");
	setmonthday(october, 8, StBridgetofSweden, feast, class_iii, "St Bridget of Sweden, W<BR>3rd Class - White");
	setmonthdaynote(october, 8, "Comm of SS Sergius, Bacchus, MM. Marcellus and Apuleius, MM");
	setmonthday(october, 9, StJohnLeonardi, feast, class_iii, "St John Leonardi, C<BR>3rd Class - White"); 	
	setmonthday(october, 10, StFrancisBorgia, feast, class_iii, "St Francis Borgia, C<BR>3rd CLass - White");
	setmonthday(october, 11, MotherhoodoftheBlessedVirgin, feast, class_ii, "Motherhood of the Blessed Virgin<BR>2nd Class - White");
	setmonthday(october, 13, StEdward, feast, class_iii, "St Edward, King of England<BR>3rd Class - White");
	setmonthday(october, 14, StCallistasI, feast, class_iii, "St Callistus I, PM<BR>3rd Class - White");
	setmonthday(october, 15, StTeresaofAvila, feast, class_iii, "St Teresa of Avila, V<BR>3rd Class - White");
	setmonthday(october, 16, StHedwig, feast, class_iii, "St Hedwig, W<BR>3rd Class - White");
	setmonthday(october, 17, StMargaretMAlacoque, feast, class_iii, "St Margaret Mary Alacoque, C<BR>3rd Class - White");
	setmonthday(october, 18, StLuke, feast, class_ii, "St Luke, EM<BR>2nd Class - Red");
	setmonthday(october, 19, StPeterofAlacantra, feast, class_iii, "St Peter of Alacantra, C<BR>3rd Class - White");
	setmonthday(october, 20, StJohnCantius, feast, class_iii, "St John Cantius, PrC<BR>3rd Class - White");
	setmonthdaynote(october, 21, "Comm of St Hilarion, Ab and St Ursula and Companions, VVMM");
	setmonthday(october, 23, StAnthonyMaryClaret, feast, class_iii, "St Anthony Mary Claret, BC<BR>3rd Class - White");
	setmonthday(october, 24, StRaphael, feast, class_iii, "St Raphael, Archangel<BR>3rd Class - White");
	setmonthdaynote(october, 25, "Comm of SS Chrysanthus and Daria, MM");
	setmonthdaynote(october, 26, "Comm of St Evaristus, PM");
	setmonthday(october, 28, SSSimonandJude, feast, class_ii, "SS Simon and Jude, Ap<BR>2nd Class - Red");

	/* November */
	setmonthday(november, 1, AllSaints, feast, class_i, "All Saints Day<BR>1st Class - White");
	setmonthday(november, 2, AllSouls, feast, class_i, "All Souls<BR>1st Class - White");
	setmonthday(november, 4, StCharlesBorromeo, feast, class_iii, "St Charles Borromeo, BC<BR>3rd Class - White");
	setmonthdaynote(november, 4, "Comm of SS Vitalis and Agricols, MM");
	setmonthdaynote(november, 8, "Comm of SS Severus, Serverianus, Carpophorus and Victorianus, MM");
	setmonthday(november, 9, DedicationoftheBasilicaofOurSavior, feast, class_ii, "Dedication of the Basilica of Our Savior<BR>2nd Class - White");
	setmonthdaynote(november, 9, "Comm of St Theodore, M");
	setmonthday(november, 10, StAndrewAvellino, feast, class_iii, "St Andrew Avellino, C<BR>3rd Class - White");
	setmonthdaynote(november, 10, "Comm of SS Tryphon, Respectus, and Nympha, MM");
	setmonthday(november, 11, StMartin, feast, class_iii, "St Martin, B<BR>3rd Class - White");
	setmonthdaynote(november, 11, "Comm is St Mennas, Soldier, M");
	setmonthday(november, 12, StMartinI, feast, class_iii, "St Martin I, PM<BR>3rd Class - Red");
	setmonthday(november, 13, StDidicus, feast, class_iii, "St Didacus, C<BR>3rd Class - White");
	setmonthday(november, 14, StJosephat, feast, class_iii, "St Jospehat, BM<BR>3rd Class - Red");
	setmonthday(november, 15, StAlberttheGreat, feast, class_iii, "St Albert the Great, BCD<BR>3rd Class - White");
	setmonthday(november, 16, StGertrude, feast, class_iii, "St Gertrude, V<BR>3rd Class - White");
	setmonthday(november, 17, StGregorytheWonderworker, feast, class_iii, "St Gregory the Wonderworker, BC<BR>3rd Class - White");
	setmonthday(november, 18, DedicationoftheBasilicasofSSPeterPaul, feast, class_iii, "Dedication of the Basicals of SS Peter and Paul<BR>3rd Class - White");
	setmonthday(november, 19, StElizabethofHungary, feast, class_iii, "St Elizabeth of Hungary, W<BR>3rd Class - White");
	setmonthdaynote(november, 19, "Comm of St Pontius, PM");
	setmonthday(november, 20, StFelixofValois, feast, class_iii, "St Felix of Valois, C<BR>3rd Class - White");
	setmonthday(november, 21, PresentationoftheBlessedVirgin, feast, class_iii, "Presentation of the Blessed Virgin Mary<BR>3rd Class - White");
	setmonthday(november, 22, StCecilia, feast, class_iii, "St Cecilia, VM<BR>3rd Class - White");
	setmonthday(november, 23, StClementI, feast, class_iii, "St Clement I, PM<BR>3rd Class - White");
	setmonthday(november, 24, StJohnoftheCross, feast, class_iii, "St John of the Cross, CD<BR>3rd Class - White");
	setmonthdaynote(november, 24, "Comm of st Chrysogonus, M");
	setmonthday(november, 25, StCatherineofAlexandria, feast, class_iii, "St Catherine of Alexandra, VM<BR>3rd Class - Red");  
	setmonthday(november, 26, StSylvester, feast, class_iii, "St Sylvester, Ab<BR>3rd Class - White");
	setmonthdaynote(november, 26, "Comm of St Peter of Alexandria, BM");
	setmonthdaynote(november, 29, "Comm of St Saturnius, M");
	setmonthday(november, 30, StAndrew, feast, class_ii, "St Andrew, Ap<BR>2nd Class - Red"); 

	/* December */
	setmonthday(december, 2, StBibiana, feast, class_iii, "St Bibiana, VM<BR>3rd Class - Red");
	setmonthday(december, 3, StFrancisXavier, feast, class_iii, "St Francis Xavier, C<BR>3rd Class - White");
	setmonthday(december, 4, StPeterChrysolgus, feast, class_iii, "St Peter Chrysologus, BCD<BR>3rd Class - White");
	setmonthdaynote(december, 4, "Comm of St Barbara, VM");
	setmonthdaynote(december, 5, "Comm of St Sabbas, Ab");
	setmonthday(december, 6, StNicholas, feast, class_iii, "St Nicholas, B<BR>3rd Class - White");
	setmonthday(december, 7, StAmbrose, feast, class_iii, "St Ambrose, BCD<BR>3rd Class - White");
	setmonthday(december, 8, ImmaculateConception, feast, class_i, "Immaculate Conception of the Blessed Virgin Mary<BR>1st Class - White");
	setmonthdaynote(december, 10, "Comm of St Melchiades, PM");
	setmonthday(december, 11, StDamasusI, feast, class_iii, "St Damasus I, PC<BR>3rd Class - White");
	setmonthday(december, 13, StLucy, feast, class_iii, "St Lucy, VM<BR>3rd Class - Red"); 
	setmonthday(december, 16, StEusebius, feast, class_iii, "St Eusebius of Vercelli, B<BR>3rd Class - White");
	setmonthday(december, 21, StThomas, feast, class_ii, "St Thomas, Ap<BR>2nd Class - Red");
	setmonthday(december, 24, VigilChristmas, vigil, class_i, "Vigil of Nativity of Our Lord<BR>1st Class - White");
	setmonthdaynote(december, 25, "Comm of St Anastasia at Second Mass, M");
	setmonthday(december, 26, StStephen, feast, class_ii, "St Stephen, First Martyr<BR>2nd Class");
	setmonthdaynote(december, 26, "Comm of Christmas");
	setmonthday(december, 27, StJohn, feast, class_ii, "St John Ab an Evang<BR>2nd Class - White");
	setmonthdaynote(december, 27, "Comm of Christmas");
	setmonthday(december, 28, HolyInnocents, feast, class_ii, "Holy Innocents, MM<BR>2nd Class - Red");
	setmonthdaynote(december, 28, "Comm of Christmas");
	setmonthdaynote(december, 29, "Comm of St Thomas Becket");
	setmonthday(december, 29, Christmas_Day5, octave, class_ii, "Fifth Day within the Octave of Christmas<BR>2nd Class - White");
	setmonthday(december, 30, Christmas_Day6, octave, class_ii, "Sixth Day within the Octave of Christmas<BR>2nd Class - White");
	setmonthday(december, 31, Christmas_Day7, octave, class_ii, "Seventh Day within the Octave of Christmas<BR>2nd Class - White");
	setmonthdaynote(december, 31, "Comm of St Sylvester I, PC");		
}

/*
 * set the temporal cycle
 */
void determinetemporalcycle(int year)
{
	month_t month;
	int day;

	determineeaster(year, &month, &day);
	setmonthday(month, day, Easter, sunday, class_i, "Easter<BR>1st Class - White");
	setmonthdayoffset(month, day, c_1d, Easter_Mon, octave, class_i, "Easter Monday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_2d, Easter_Tue, octave, class_i, "Easter Tuesday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_3d, Easter_Wed, octave, class_i, "Easter Wednesday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_4d, Easter_Thur, octave, class_i, "Easter Thursday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_5d, Easter_Fri, octave, class_i, "Easter Friday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_6d, Easter_Sat, octave, class_i, "Easter Saturday<BR>1st Class - White");
	setmonthdayoffset(month, day, c_1w, Easter_1, sunday, class_i, "First Sunday after Easter<BR>1st Class White");
	setmonthdayoffset(month, day, c_2w, Easter_2, sunday, class_ii, "Second Sunday after Easter<BR>2nd Class White");
	setmonthdayoffset(month, day, c_3w, Easter_3, sunday, class_ii, "Third Sunday after Easter<BR>2nd Class - White");
	setmonthdayoffset(month, day, c_4w, Easter_4, sunday, class_ii, "Fourth Sunday after Easter<BR>2nd Class - White");
	setmonthdayoffset(month, day, c_5w, Easter_5, sunday, class_ii, "Fifth Sunday after Easter<BR>2nd Class - White");
	setmonthdayoffset(month, day, c_5w + c_4d, Ascension, feast, class_ii, "Ascension<BR>1st Class - White");
	setmonthdayoffset(month, day, c_6w, Ascension_1, sunday, class_ii, "Sunday after Ascension<BR>2nd Class White");
	setmonthdayoffset(month, day, c_8w, Trinity, sunday, class_i, "Trinity Sunday<BR>1st Class - White");
	setmonthdayoffset(month, day, -c_8w, Sexagesima, sunday, class_ii, "Sexasegima<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -c_9w, Septuagesima, sunday, class_ii, "Septuagesima<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -c_7w, Quinquagesima, sunday, class_ii, "Quinquagesima<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -c_7w + c_3d, AshWednesday, feria, class_i, "Ash Wednesday<BR>1st Class - Violet");
	setmonthdayoffset(month, day, -c_6w, Lent_1, sunday, class_i, "First Sunday of Lent<BR>1st Class - Violet");
	setmonthdayoffset(month, day, -(c_5w + c_4d), Lent_WedEmber, feria, class_ii, "Ember Wednesday in Lent<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -(c_5w + c_2d), Lent_FriEmber, feria, class_ii, "Ember Friday in Lent<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -(c_5w + c_1d), Lent_SatEmber, feria, class_ii, "Ember Saturday in Lent<BR>2nd Class - Violet");
	setmonthdayoffset(month, day, -c_5w, Lent_2, sunday, class_i, "Second Sunday of Lent<BR>1st Class - Violet");
	setmonthdayoffset(month, day, -c_4w, Lent_3, sunday, class_i, "Third Sunday of Lent<BR>1st Class - Violet");
	setmonthdayoffset(month, day, -c_3w, Lent_4, sunday, class_i, "Laetare Sunday<BR>1st Class - Rose");
	setmonthdayoffset(month, day, -c_2w, PassionSunday, sunday, class_i, "Passion Sunday<BR>1st Class - Violet");
	setmonthdayoffset(month, day, -c_1w, PalmSunday, sunday, class_i, "Palm Sunday<BR>1st Class - Red and Violet");
	setmonthdayoffset(month, day, -c_6d, HolyMonday, feria, class_i, "Monday in Holy Week<BR>1st Class - White and Violet");
	setmonthdayoffset(month, day, -c_5d, HolyTuesday, feria, class_i, "Tuesday in Holy Week<BR>1st Class - White and Violet");
	setmonthdayoffset(month, day, -c_4d, SpyWednesday, feria, class_i, "Spy Wednesday<BR>1st Class - White and Violet");
	setmonthdayoffset(month, day, -c_3d, MaundyThursday, feria, class_i, "Maundy Thursday<BR>1st Class - White and Violet");
	setmonthdayoffset(month, day, -c_2d, GoodFriday, feria, class_i, "Good Friday<BR>1st Class - Black and Violet");
	setmonthdayoffset(month, day, -c_1d, HolySaturday, feria, class_i, "Holy Saturday<BR>1st Class - White and Violet");
	determineadvent(year);
	determineepiphany(year);
	determinechristtheking(year);
	determinepentecost(month, day);
	determinerogationdays(year, month, day);
}

void determinenovenas(int year)
{
	month_t month;
	int day;

	setmonthdayoffsetdevotion(january, 24, 0, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 1</a>");
	setmonthdayoffsetdevotion(january, 24, 1, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 2</a>");
	setmonthdayoffsetdevotion(january, 24, 2, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 3</a>");
	setmonthdayoffsetdevotion(january, 24, 3, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 4</a>");
	setmonthdayoffsetdevotion(january, 24, 4, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 5</a>");
	setmonthdayoffsetdevotion(january, 24, 5, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 6</a>");
	setmonthdayoffsetdevotion(january, 24, 6, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 7</a>");
	setmonthdayoffsetdevotion(january, 24, 7, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 8</a>");
	setmonthdayoffsetdevotion(january, 24, 8, "<a href=\"http://www.fisheaters.com/novenas.html#olgs\">Our Lady of Good Success Day 9</a>");


	setmonthdayoffsetdevotion(march, 10, 0, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 1</a>");
	setmonthdayoffsetdevotion(march, 10, 1, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 2</a>");
	setmonthdayoffsetdevotion(march, 10, 2, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 3</a>");
	setmonthdayoffsetdevotion(march, 10, 3, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 4</a>");
	setmonthdayoffsetdevotion(march, 10, 4, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 5</a>");
	setmonthdayoffsetdevotion(march, 10, 5, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 6</a>");
	setmonthdayoffsetdevotion(march, 10, 6, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 7</a>");
	setmonthdayoffsetdevotion(march, 10, 7, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 8</a>");
	setmonthdayoffsetdevotion(march, 10, 8, "<a href=\"http://www.fisheaters.com/novenas.html#joseph\">St Joseph Day 9</a>");

	determineeaster(year, &month, &day);

	setmonthdayoffsetdevotion(month, day, c_5w + c_5d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 1</a>");
	setmonthdayoffsetdevotion(month, day, c_5w + c_6d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 2</a>");
	setmonthdayoffsetdevotion(month, day, c_6w, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 3</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_1d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 4</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_2d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 5</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_3d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 6</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_4d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 7</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_5d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 8</a>");
	setmonthdayoffsetdevotion(month, day, c_6w + c_6d, "<a href=\"http://www.fisheaters.com/novenas.html#holyghost\">Holy Ghost Day 9</a>");

	setmonthdayoffsetdevotion(july, 17, 0, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 1</a>");
	setmonthdayoffsetdevotion(july, 17, 1, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 2</a>");
	setmonthdayoffsetdevotion(july, 17, 2, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 3</a>");
	setmonthdayoffsetdevotion(july, 17, 3, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 4</a>");
	setmonthdayoffsetdevotion(july, 17, 4, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 5</a>");
	setmonthdayoffsetdevotion(july, 17, 5, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 6</a>");
	setmonthdayoffsetdevotion(july, 17, 6, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 7</a>");
	setmonthdayoffsetdevotion(july, 17, 7, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 8</a>");
	setmonthdayoffsetdevotion(july, 17, 8, "<a href=\"http://www.fisheaters.com/novenas.html#Ann\">St Anne Day 9</a>");

	setmonthdayoffsetdevotion(october, 20, 0, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 1</a>");
	setmonthdayoffsetdevotion(october, 20, 1, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 2</a>");
	setmonthdayoffsetdevotion(october, 20, 2, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 3</a>");
	setmonthdayoffsetdevotion(october, 20, 3, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 4</a>");
	setmonthdayoffsetdevotion(october, 20, 4, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 5</a>");
	setmonthdayoffsetdevotion(october, 20, 5, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 6</a>");
	setmonthdayoffsetdevotion(october, 20, 6, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 7</a>");
	setmonthdayoffsetdevotion(october, 20, 7, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 8</a>");
	setmonthdayoffsetdevotion(october, 20, 8, "<a href=\"http://www.fisheaters.com/novenas.html#teresa\">St Teresa of Avila Day 9</a>");


	setmonthdayoffsetdevotion(october, 20, 0, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 1</a>");
	setmonthdayoffsetdevotion(october, 20, 1, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 2</a>");
	setmonthdayoffsetdevotion(october, 20, 2, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 3</a>");
	setmonthdayoffsetdevotion(october, 20, 3, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 4</a>");
	setmonthdayoffsetdevotion(october, 20, 4, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 5</a>");
	setmonthdayoffsetdevotion(october, 20, 5, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 6</a>");
	setmonthdayoffsetdevotion(october, 20, 6, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 7</a>");
	setmonthdayoffsetdevotion(october, 20, 7, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 8</a>");
	setmonthdayoffsetdevotion(october, 20, 8, "<a href=\"http://www.fisheaters.com/novenas.html#jude\">St Jude Day 9</a>");

	setmonthdayoffsetdevotion(december, 28, 0, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 1</a>");
	setmonthdayoffsetdevotion(december, 28, 1, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 2</a>");
	setmonthdayoffsetdevotion(december, 28, 2, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 3</a>");
	setmonthdayoffsetdevotion(december, 28, 3, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 4</a>");
	setmonthdayoffsetdevotion(january, 1, 0, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 5</a>");
	setmonthdayoffsetdevotion(january, 1, 1, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 6</a>");
	setmonthdayoffsetdevotion(january, 1, 2, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 7</a>");
	setmonthdayoffsetdevotion(january, 1, 3, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 8</a>");
	setmonthdayoffsetdevotion(january, 1, 4, "<a href=\"http://www.fisheaters.com/novenas.html#magi\">Magi Day 9</a>");
}

int main(int argc, char** argv)
{
	int year;
	dayofweek_t daycode;

	memset(g_liturgicalday, 0, sizeof(g_liturgicalday));

	if (argc != 2)
	{
		printf("%s year\n", argv[0]);
		exit(-1);
	}

	sscanf(argv[1], "%d", &year);

	determineleapyear(year);
	daycode = determinedaycode(january, 1, year);

	determinesanctoralcycle(year);
	determinetemporalcycle(year);
	determinenovenas(year);


	calendarhtml(year, daycode);

	return (0);
}
