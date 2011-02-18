/*	Better Oblivion Sorting Software
	
	Quick and Dirty Load Order Utility
	(Making C++ look like the scripting language it isn't.)

    Copyright (C) 2009-2010  Random/Random007/jpearce & the BOSS development team
    http://creativecommons.org/licenses/by-nc-nd/3.0/

	$Revision: 2188 $, $Date: 2011-01-20 10:05:16 +0000 (Thu, 20 Jan 2011) $
*/

//Contains the modlist (inc. masterlist) and userlist data structures, and some helpful functions for dealing with them.
//DOES NOT CONTAIN ANYTHING TO DO WITH THE FILE PARSERS.

#ifndef __BOSS_LISTS_H__
#define __BOSS_LISTS_H__

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_NO_DEPRECATED
#endif

#include <string>
#include <vector>
#include "boost/filesystem.hpp"

namespace boss {
	using namespace std;
	namespace fs = boost::filesystem;


	//modlist and masterlist data structure
	enum itemType {
		MOD,
		GROUPBEGIN,
		GROUPEND
	};

	struct message {
		string key;
		string data;
	};

	//items can be mods, group beginning markers and group end markers. All three have a type and a name, but the messages vector is only 
	//non-zero size if the item is a mod with messages attached.
	struct item {
		itemType type;
		fs::path name;
		vector<message> messages;
	};

	struct modlist {
		vector<item> items;
	};


	//Userlist data structure.
	struct line {
		string key;
		string object;
	};

	struct rule {
		vector<line> lines;
	};

	struct userlist {
		vector<rule> rules;
	};

	/* NOTES: USERLIST GRAMMAR RULES.

	Below are the grammar rules that the parser must follow. Noted here until they are implemented.

	1. Userlist must be encoded in UTF-8 or UTF-8 compatible ANSI. Use checking functions and abort the userlist if mangled line found.
	2. All lines must contain a recognised keyword and an object. If one is missing or unrecognised, abort the rule.
	3. If a rule object is a mod, it must be installed. If not, abort the rule.
	4. Groups cannot be added. If a rule tries, abort it.
	5. The 'ESMs' group cannot be sorted. If a rule tries, abort it.
	6. The game's main master file cannot be sorted. If a rule tries, abort it.
	7. A rule with a FOR rule keyword must not contain a sort line. If a rule tries, ignore the line and print a warning message.
	8. A rule may not reference a mod and a group unless its sort keyword is TOP or BOTTOM and the mod is the rule object.  If a rule tries, abort it.
	9. No group may be sorted before the 'ESMs' group. If a rule tries, abort it.
	10. No mod may be sorted before the game's main master file. If a rule tries, abort it.
	11. No mod may be inserted into the top of the 'ESMs' group. If a rule tries, abort it.
	12. No rule can insert a group into anything or insert anything into a mod. If a rule tries, abort it.
	13. No rule may attach a message to a group. If a rule tries, abort it.
	14. The first line of a rule must be a rule line. If there is a valid line before the first rule line, ignore it and print a warning message.

	*/


	//Helpful functions.

	//Find a mod by name. Will also find the starting position of a group.
	int GetModPos(modlist list, string filename);

	//Find the end of a group by name.
	int GetGroupEndPos(modlist list, string groupname);

	//Date comparison, used for sorting mods in modlist.
	bool SortModsByDate(item mod1, item mod2);

	//Build modlist (the one that gets saved to file, not the masterlist).
	//Adds mods in directory to modlist in date order (AKA load order).
	void BuildModlist(modlist &list);

	//Save the modlist (not masterlist) to a file, printing out all the information in the data structure.
	//This will likely just be one group and the list of filenames in that group, if it's the modlist.
	//However, if used on the masterlist, could prove useful for debugging the parser.
	void SaveModlist(modlist list, fs::path file);
}

#endif