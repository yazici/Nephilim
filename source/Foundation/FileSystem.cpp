#include <Nephilim/Foundation/FileSystem.h>
#include <Nephilim/Foundation/Path.h>
#include <Nephilim/Foundation/File.h>
#include <Nephilim/Foundation/Logging.h>

#ifdef NEPHILIM_WINDOWS
#include <windows.h>
#include <CommDlg.h>

#ifndef _WIN32_IE
#define _WIN32_IE 0x0550
#endif

#include <Shlobj.h>
#ifdef SetCurrentDirectory
#undef SetCurrentDirectory
#endif
#elif defined NEPHILIM_ANDROID
#include <Nephilim/Foundation/AndroidWrapper.h>
#include <Nephilim/Foundation/Logging.h>
#endif

NEPHILIM_NS_BEGIN

namespace fs
{

	bool copyFile(const String& source, const String& destination)
	{
		File src(source, IODevice::BinaryRead);
		File dst(destination, IODevice::BinaryWrite);

		if(src && dst)
		{
			FileOps::copy(src, dst);
			return true;
		}
		else
		{
			Log("Failed to copy '%s' to '%s'", source.c_str(), destination.c_str());
			return false;
		}
	}

	/// Copy the contents of a directory to another
	bool copyDirectoryContents(const String& source, const String& destination)
	{
		bool copiedAll = true;
		StringList fileList = FileSystem::scanDirectory(source, "*", false);
		for(std::size_t i = 0; i < fileList.size(); ++i)
		{
			Path shortName(fileList[i]);
			if(!copyFile(fileList[i], destination + shortName.getFileName()))
			{
				copiedAll = false;
			}
		}
		return copiedAll;
	}

	/// Copy the contents of a directory to another
	bool copyDirectoryContentsRecursively(const String& source, const String& destination)
	{
		bool copiedAll = true;
		StringList fileList = FileSystem::scanDirectory(source, "*", true);
		for(std::size_t i = 0; i < fileList.size(); ++i)
		{
			Path shortName(fileList[i]);
			//Log("Read file: %s", fileList[i].c_str());

			//Log("Proper relative path: %s", shortName.getRelativePathTo(source).c_str());


			ensureDirectory(destination + shortName.getRelativePathTo(source));

			if(!copyFile(fileList[i], destination + shortName.getRelativePathTo(source)))
			{
				copiedAll = false;
			}
		}
		return copiedAll;
	}


	/// Creates all needed directories to make the path valid
	bool ensureDirectory(const String& directory)
	{
		//Log("Need to ensure: %s", directory.c_str());

		Path path(directory);

		StringList pathElements = path.m_path.split('/');

		/*if(path.isFile())
			pathElements.pop_back();*/

		String currentDir = pathElements[0];
		for(std::size_t i = 1; i < pathElements.size(); ++i)
		{
			FileSystem::makeDirectory(currentDir);
			//Log("Creating %s", currentDir.c_str());
			currentDir += "/" + pathElements[i];
		}

		return true;
	}
}








	String FileSystem::myCurrentDirectory;

#ifdef NEPHILIM_WINDOWS
	std::wstring wide_string_from_string(std::string str){
		return std::wstring(str.begin(), str.end());
	}
#endif

/// Load a dialog
String FileSystem::loadFileDialog(){
#ifdef NEPHILIM_WINDOWS
		char Filestring[256];
		String returnstring;

		OPENFILENAMEA opf;
		opf.hwndOwner = NULL;
		opf.lpstrFilter = "";
		opf.lpstrCustomFilter = 0;
		opf.nMaxCustFilter = 0L;
		opf.nFilterIndex = 1L;
		opf.lpstrFile = Filestring;
		opf.lpstrFile[0] = '\0';
		opf.nMaxFile = 256;
		opf.lpstrFileTitle = 0;
		opf.nMaxFileTitle=50;
		opf.lpstrInitialDir = "K:\\";
		opf.lpstrTitle = "Nephilim";
		opf.nFileOffset = 0;
		opf.nFileExtension = 0;
		opf.lpstrDefExt = "*.*";
		opf.lpfnHook = NULL;
		opf.lCustData = 0;
		opf.Flags = (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR) & ~OFN_ALLOWMULTISELECT ;
		opf.lStructSize = sizeof(OPENFILENAME);

		if(GetOpenFileNameA(&opf))
		{
			std::string s = static_cast<std::string>(opf.lpstrFile);
			returnstring.assign(s.begin(), s.end());
		}

		return returnstring;
#else
	return "";
#endif
	return "";
};

/// Testing \todo make
String FileSystem::saveFileDialog(){
#ifdef NEPHILIM_WINDOWS
	OPENFILENAME ofn;
	String result;
	/*
	wchar_t saveFileName[MAX_PATH] = L"";

	ZeroMemory( &ofn, sizeof( ofn ) );

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Script file (*.as)\0*.as\0";
	ofn.lpstrFile = saveFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = L"as";
	ofn.Flags  = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
	ofn.lpstrTitle = L"Save File";


	if(GetSaveFileName(&ofn)){
		std::wstring s = static_cast<std::wstring>(ofn.lpstrFile);
		result.assign(s.begin(), s.end());
	}
	*/
	return result;
	//	wcscpy(file,saveFileName);
#else
	return "";
#endif
	return "";
};

String FileSystem::getDocumentsDirectory(){
	String ActualStorageDirectory;
#ifdef NEPHILIM_WINDOWS
	ActualStorageDirectory.resize(1024);

	if(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, &ActualStorageDirectory[0]) < 0)
	{
		ActualStorageDirectory.clear();
	}
	else
	{
		ActualStorageDirectory.resize(strlen(ActualStorageDirectory.c_str()));
	};
#endif

	ActualStorageDirectory.replaceCharacter('\\', '/');
	return ActualStorageDirectory;

};


/// Get the path of the executable - could be similar to C:/Games/ or /home/games/
String FileSystem::getExecutableDirectory(){
	String path;
#ifdef NEPHILIM_WINDOWS
	LPSTR lpStr = new char[2048];
	GetModuleFileNameA(NULL,lpStr,2048 );
	path = lpStr;
	delete lpStr;

	path.removeUntilReverse('\\');
	path.replaceCharacter('\\', '/');

	path += "/";
#endif

	return path;
};

StringList FileSystem::scanDirectory(const String &Directory, const String &Extension, bool Recursive)
{
	StringList Files;
		{
		#ifdef NEPHILIM_WINDOWS ////////////////////////////////////////////////////WINDOWS CONFIG
			String Query = Directory + String("\\*.") + Extension;
			WIN32_FIND_DATAA FileData;
			HANDLE hFindHandle = INVALID_HANDLE_VALUE;
			hFindHandle = FindFirstFileA(Query.c_str(), &FileData);

			// Let's get in the sub directories
			if (Recursive)
			{
				// Let's get the directory listing
				StringList DirectoryList = directoryList(Directory, true);
				Log("Found %d dirs", DirectoryList.size());

				for (std::size_t i = 0; i < DirectoryList.size(); ++i)
				{
					//Log("Searching in subdir %s", DirectoryList[i].c_str());

					StringList SubDirectoryResults = scanDirectory(DirectoryList[i], Extension, Recursive);
					Files.insert(Files.end(), SubDirectoryResults.begin(), SubDirectoryResults.end());
				}
			}

			// Are there any files here?
			if(hFindHandle == INVALID_HANDLE_VALUE)
				return Files;

			String FileName(FileData.cFileName);

			if(FileName[0] != '.')
			{
				if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(Recursive)
					{

						String SubDirectoryFull = Directory + String("\\") + String(FileData.cFileName);
						Log("Scanning subdir %s", SubDirectoryFull.c_str());

						StringList t(scanDirectory(Directory + String("\\") +
							String(FileData.cFileName), Extension, Recursive));

						Files.insert(Files.end(), t.begin(), t.end());
					};
				}
				else
				{
					String FileName(FileData.cFileName);
					Files.push_back(Directory + String("/") + FileName);
				};
			};



			while(FindNextFileA(hFindHandle, &FileData))
			{
				FileName = FileData.cFileName;

				if(FileName[0] != '.')
				{
					if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log("Found a dir");
						if(Recursive)
						{
							String SubDirectoryFull = Directory + String("\\") + String(FileData.cFileName);
							Log("Scanning subdir %s", SubDirectoryFull.c_str());

							StringList t(scanDirectory(Directory + String("\\") +
								String(FileData.cFileName), Extension, Recursive));

							Files.insert(Files.end(), t.begin(), t.end());
						}
					}
					else
					{
						Files.insert(Files.end(), Directory + String("/") + FileName);
					}
				}
			};

	
		
		// Close
		FindClose(hFindHandle);


		#elif defined(PARABOLA_PLATFORM_LINUX) || defined(PARABOLA_PLATFORM_MAC)///////////////////////MAC|LINUX
			DIR *Root = opendir (Directory);

			if(Root == nullptr)
			{
				return Files;
			}

			dirent *Entry = readdir(Root);

			while(Entry != nullptr)
			{
				String FileName(Entry->d_name);

				if(FileName[0] != '.')
				{
					if(Entry->d_type == DT_DIR)
					{
						if(Recursive)
						{
							std::vector<String> t(scanDirectory(Directory + String("/") +
								String(FileData.cFileName), Extension, Recursive));

							Files.insert(Files.end(), t.begin(), t.end());
						};
					}
					else if(Entry->d_type == DT_REG)
					{
						Files.push_back(Directory + String("/") + FileName);
					};
				};

				Entry = readdir(Root);
			}

			closedir(Root);
	#elif defined PARABOLA_ANDROID
		Files = AndroidWrapper::getAssetList(Directory);


	#endif
		}

		return Files;
	}


	bool FileSystem::isDirectory(String Directory){
		#ifdef NEPHILIM_WINDOWS ////////////////////////////////////////////////////WINDOWS CONFIG
			String Query = Directory;
			WIN32_FIND_DATAA FileData;
			HANDLE hFindHandle = INVALID_HANDLE_VALUE;
			hFindHandle = FindFirstFileA(Query.c_str(), &FileData);

			if(hFindHandle == INVALID_HANDLE_VALUE)
				return false;

			String FileName(FileData.cFileName);
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				return true;
			else
				return false;

		#else

		#endif
	};


	bool FileSystem::isFile(String fileName){
		#ifdef NEPHILIM_WINDOWS ////////////////////////////////////////////////////WINDOWS CONFIG
			String Query = fileName;
			WIN32_FIND_DATAA FileData;
			HANDLE hFindHandle = INVALID_HANDLE_VALUE;
			hFindHandle = FindFirstFileA(Query.c_str(), &FileData);

			if(hFindHandle == INVALID_HANDLE_VALUE)
				return false;

			String FileName(FileData.cFileName);
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
				return true;
			else
				return false;

		#else

		#endif
	}


	StringList FileSystem::directoryList(String Where, bool KeepRelativePath){
		StringList Files;
		{
		#ifdef NEPHILIM_WINDOWS ////////////////////////////////////////////////////WINDOWS CONFIG
			String Query = Where + String("\\*");
			WIN32_FIND_DATAA FileData;
			HANDLE hFindHandle = INVALID_HANDLE_VALUE;
			hFindHandle = FindFirstFileA(Query.c_str(), &FileData);

			if(hFindHandle == INVALID_HANDLE_VALUE)
				return Files;

			String FileName(FileData.cFileName);

			if(FileName[0] != '.' && FileName != "..")
			{
				if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					String FileName(FileData.cFileName);
					if(KeepRelativePath)
						Files.push_back(Where + String("/") + FileName);
					else
						Files.push_back(FileData.cFileName);

				}

			};

			while(FindNextFileA(hFindHandle, &FileData))
			{
				FileName = FileData.cFileName;

				if(FileName[0] != '.' && FileName != "..")
				{
					if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						String FileName(FileData.cFileName);
						if(KeepRelativePath)
							Files.push_back(Where + String("/") + FileName);
						else
							Files.push_back(FileData.cFileName);
					}

				};
			};

			FindClose(hFindHandle);
		#elif defined(PARABOLA_PLATFORM_LINUX) || defined(PARABOLA_PLATFORM_MAC)///////////////////////MAC|LINUX
			DIR *Root = opendir (Directory);

			if(Root == nullptr)
			{
				return Files;
			}

			dirent *Entry = readdir(Root);

			while(Entry != nullptr)
			{
				String FileName(Entry->d_name);

				if(FileName[0] != '.')
				{
					if(Entry->d_type == DT_DIR)
					{
						if(Recursive)
						{
							std::vector<String> t(scanDirectory(Directory + String("/") +
								String(FileData.cFileName), Extension, Recursive));

							Files.insert(Files.end(), t.begin(), t.end());
						};
					}
					else if(Entry->d_type == DT_REG)
					{
						Files.push_back(Directory + String("/") + FileName);
					};
				};

				Entry = readdir(Root);
			}

			closedir(Root);
#elif defined PARABOLA_ANDROID



	#endif
		}

		return Files;
	};

bool FileSystem::makeDirectory(String Name)
{
#ifdef NEPHILIM_WINDOWS
		//if(!CreateDirectoryA((LPCWSTR)wide_string_from_string(Name).c_str(), LPSECURITY_ATTRIBUTES()))
		if(!CreateDirectoryA((LPCSTR)Name.c_str(), LPSECURITY_ATTRIBUTES()))
			return false;
#elif defined PARABOLA_ANDROID
		TESTLOG("makeDirectory\n");
		return AndroidWrapper::createDirectory(Name);
#else
		//TESTLOG("makeDirectory NOT android\n");
		//_mkdir(Name.c_str());
	//	mkdir(Name.c_str(), 0770);
#endif
	return true;
}

	String FileSystem::currentDirectory(){
		return myCurrentDirectory;
	}


NEPHILIM_NS_END
