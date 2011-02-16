/*	Better Oblivion Sorting Software
	
	Quick and Dirty Load Order Utility
	(Making C++ look like the scripting language it isn't.)

	Copyright (C) 2009-2010  Random/Random007/jpearce & the BOSS development team
	http://creativecommons.org/licenses/by-nc-nd/3.0/

	$Revision$, $Date$
*/

#include "Updater.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


namespace boss {
	namespace fs = boost::filesystem;
	using namespace std;

	int writer(char *data, size_t size, size_t nmemb, string *buffer){
		int result = 0;
		if(buffer != NULL) {
			buffer -> append(data, size * nmemb);
			result = size * nmemb;
		}
		return result;
	} 

	int UpdateMasterlist(int game) {
		const char *url;							//Masterlist file url
		char cbuffer[4096];
		char errbuff[CURL_ERROR_SIZE];
		CURL *curl;									//cURL handle
		string buffer,revision,newline,line;		//A bunch of strings.
		size_t start=-1,end;								//Position holders for trimming strings.
		CURLcode ret;
		ifstream mlist;								//Input stream.
		ofstream out;								//Output stream.
		const string SVN_REVISION_KW = "$" "Revision" "$";                   // Left as separated parts to avoid keyword expansion
		const string SVN_DATE_KW = "$" "Date" "$";                           // Left as separated parts to avoid keyword expansion
		const string SVN_CHANGEDBY_KW= "$" "LastChangedBy" "$";              // Left as separated parts to avoid keyword expansion
		string oldline = "? Masterlist Information: "+SVN_REVISION_KW+", "+SVN_DATE_KW+", "+SVN_CHANGEDBY_KW;

		//Which masterlist to get?
		if (game == 1) url = "http://better-oblivion-sorting-software.googlecode.com/svn/data/boss-oblivion/masterlist.txt";
		else if (game == 2) url = "http://better-oblivion-sorting-software.googlecode.com/svn/data/boss-fallout/masterlist.txt";
		else if (game == 3) url = "http://better-oblivion-sorting-software.googlecode.com/svn/data/boss-nehrim/masterlist.txt";
		else url = "http://better-oblivion-sorting-software.googlecode.com/svn/data/boss-fallout-nv/masterlist.txt";
		
		//curl will be used to get stuff from the internet, so initialise it.
		curl = curl_easy_init();
		if (!curl) 
			throw boss_error() << err_detail("Curl could not be initialised.");
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuff);	//Set error buffer for curl.
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);		//Set connection timeout to 10s.
		
		//Get revision number from http://code.google.com/p/better-oblivion-sorting-software/source/browse/#svn page text.
		curl_easy_setopt(curl, CURLOPT_URL, "http://code.google.com/p/better-oblivion-sorting-software/source/browse/#svn");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);	
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &revision );
		ret = curl_easy_perform(curl);
		if (ret!=CURLE_OK)
			throw boss_error() << err_detail(errbuff);
		
		//Extract revision number from page text.
		if (game == 1) start = revision.find("boss-oblivion");
		else if (game == 2) start = revision.find("boss-fallout");
		else if (game == 3) start = revision.find("boss-nehrim");
		else if (game == 4) start = revision.find("boss-fallout-nv");
		if (start == string::npos)
			throw boss_error() << err_detail("Cannot find online masterlist revision number.");
		start = revision.find("\"masterlist.txt\"", start);
		start = revision.find("B\",\"", start) + 4; 
		if (start == string::npos)
			throw boss_error() << err_detail("Cannot find online masterlist revision number.");
		end = revision.find("\"",start) - start;
		if (end == string::npos)
			throw boss_error() << err_detail("Cannot find online masterlist revision number.");
		revision = revision.substr(start,end);
		newline = "? Masterlist Revision: "+revision;

		//Compare remote revision to current masterlist revision - if identical don't waste time/bandwidth updating it.
		if (fs::exists(masterlist_path)) {
			mlist.open(masterlist_path.c_str());
			if (mlist.fail())
				throw boss_error() << err_detail("Masterlist cannot be opened.");
			while (!mlist.eof()) {
				mlist.getline(cbuffer,4096);
				line=cbuffer;
				if (line.find("? Masterlist") != string::npos) {
					if (line.find(newline) != string::npos) {
						return 0;  //Masterlist already at latest revision.
					} else break;
				}
			}
			mlist.close();
		}

		// Use curl to get latest masterlist file from SVN repository
		//Change url and data structure settings, writefunction setting is retained.
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_CRLF, 1);
		ret = curl_easy_perform(curl);
		if (ret!=CURLE_OK)
			throw boss_error() << err_detail(errbuff);

		//Clean up and close curl handle now that it's finished with.
		curl_easy_cleanup(curl);

		//Replace SVN keywords with revision number and replace current masterlist, or write a new one if it doesn't already exist.
		size_t pos = buffer.find(oldline);
		if (pos != string::npos)
			buffer.replace(pos,oldline.length(),newline);
		out.open(masterlist_path.c_str(), ios_base::trunc);
		if (out.fail())
			throw boss_error() << err_detail("Masterlist cannot be opened.");
		out << buffer;
		out.close();

		//Return revision number.
		return atoi(revision.c_str());
	}
}