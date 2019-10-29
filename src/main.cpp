#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "imgui/imgui.hh"
#include "imgui/imgui_impl_sdl.hh"

#include <functional>
#include "spline.h"
#include "spline_light.h"
#if defined(__EMSCRIPTEN__)
#define DIR_PATH ("/dir")
#include <emscripten.h>
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL.h>
#include <SDL_opengles2.h>
#else
#define DIR_PATH ("/tmp/dir")
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif
#include <experimental/filesystem> 

#define FILE_MAX_SIZE (20000)
#define WIDTH (1000)
#define HEIGHT (1000)
namespace fs =std::experimental::filesystem::v1;


std::vector<std::vector<float>> values;
std::vector<float> vec;
std::vector<SOURCE_TYPE> tBuf;
bool callback(SOURCE_TYPE* data, size_t size, size_t imf)
{
	std::cout << "Imf=" << imf << std::endl;
	values.push_back(std::vector<float>());
	values[imf - 1].resize(size);
	
	for (size_t i = 0; i < size; i++)
	{
		values[imf-1][i] = data[i];
	}
//	if(imf >=3)
//		return false;//stop execution after 3rd IMF
	return true;
};

SDL_Window* window_;
SDL_GLContext context_;
std::vector<std::string> listOfFiles;
std::vector<std::string>& getListOfFiles()
{
	return listOfFiles;
}
int getXBottom(const std::vector<float>& v)
{
	int res = std::numeric_limits<int>::max();
	for(auto it : v)
	{
		if(it < res)
			res = it;
	}
	return res-1;
}
int getXTop(const std::vector<float>& v)
{
	int res = std::numeric_limits<int>::min();
	for(auto it : v)
	{
		if(it > res)
			res = it;
	}
	return res+1;
}
void init()
{
	for (auto & p : fs::directory_iterator(DIR_PATH))
	{
		std::string path = p.path();
		size_t pos = path.rfind('/')+1;
		size_t npos = path.rfind('.')-pos;
		listOfFiles.push_back(path.substr(pos, npos));
	}
        ImGui::CreateContext();
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
		std::cout << "SDL_Init error" << std::endl;
               return;
        }
					
	window_ = SDL_CreateWindow("", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

        context_ = SDL_GL_CreateContext(window_);
        ImGui_ImplSdl_Init(window_);
}
int processFile(std::string file, size_t algType, std::vector<float>& result)
{
		std::string filePath = DIR_PATH;
	        filePath += "/" + file + ".txt";
		std::cout << "Process file=" << filePath << std::endl;
		std::fstream fs;
		fs.open( filePath, std::fstream::in );
		if( !fs.good() )
		{
			std::cout << "File doesn't exist" << std::endl;
			return 0;
		}
		std::string line;
		size_t fileLength=0;
		values.clear();
		vec.clear();
		while ( std::getline(fs, line ))
		{
			std::string lline = line;
			double val = 0;
			val = std::stod(lline);
			fileLength++;
			vec.push_back( val );
			if (fileLength >= FILE_MAX_SIZE)
				break;
		}
		size_t quntN = vec.size();
		std::vector<SOURCE_TYPE> cvec( &vec[0], &vec[quntN] );
		std::vector<double> res;
		ContextE ce;
		alloc(&ce, FILE_MAX_SIZE);//you can init pointers by static buffers if you need, or write your own allocator for it
		clearFlyC(&ce.minfc);
		clearFlyC(&ce.maxfc);
		FlyContextE fce;
		tBuf.resize(vec.size());
		res.resize(vec.size());
		if ( vec.size() == quntN )
			vec.push_back( *vec.rbegin() );
		clock_t begin = clock();

	int procres = 0;
	if(algType == 1)
	{
		std::cout << "light spline" << std::endl;
		procres = rParabEmd__LFlyEmb(&cvec[0], &tBuf[0], cvec.size(), 15, 15, &fce, callback);
	}
	else
	{
		std::cout << "classic spline" << std::endl;
		procres = rParabEmd__LEmb(&cvec[0], cvec.size(), 15, 15, &ce, &res[0], callback);
	}
		result = cvec;
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		std::cout << " Res=" << procres <<  " Time spent=" << time_spent << std::endl;
		release(&ce);
	return 0;
}
std::vector<float> result;
void main_loop()
{
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_SetWindowPosition(window_, 0, 0);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0xF1, 0xF1, 0xF1,255));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(0xF1, 0xF1, 0xF1,255));
	ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(0, 0, 0, 255));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

        SDL_GL_MakeCurrent(window_, context_);
        ImGui_ImplSdl_NewFrame(window_);
        ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT), ImGuiCond_Appearing);

	bool showWin = 1;
	ImGui::Begin("", &showWin, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	static std::string current_item;

	if (ImGui::BeginCombo("##combo", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
	{
        	auto files = getListOfFiles();
	    for (int n = 0; n < files.size(); n++)
	    {
		    bool is_selected = (current_item.compare(files[n])==0); // You can store your selection however you want, outside or inside your objects
	        if (ImGui::Selectable(files[n].c_str(), is_selected))
		{
	            current_item = files[n];
		}
	            if (is_selected)
		    {
	               ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		    }
	   }
	   ImGui::EndCombo();
	}
	static int algType = 0;
        ImGui::RadioButton("classic spline", &algType, 0); ImGui::SameLine();
        ImGui::RadioButton("lightweight spline", &algType, 1); 

	ImVec2 btnsize(100, 20);
	if (ImGui::Button("Calculate", btnsize))
	{
		result.clear();
		processFile(current_item, algType, result);
	}
	       
	size_t lineH = (HEIGHT-300)/((values.size()>0)?values.size():1);
	if(lineH >80)
		lineH = 80;
	ImGui::PlotLines("Source", &vec[0], vec.size(), 0, NULL, getXBottom(vec), getXTop(vec), ImVec2(0, lineH));
	for(int i =0; i< values.size();i ++)
	{
		std::stringstream hdr;
	        hdr << "Imf(";
		hdr << i+1;
		hdr << ")";
		std::string hdrStr = hdr.str().c_str();
		ImGui::PlotLines(hdrStr.c_str(), &values[i][0], values[i].size(), 0, NULL, getXBottom(values[i]), getXTop(values[i]), ImVec2(0, lineH));
	}
	ImGui::PlotLines("(Result)", &result[0], result.size(), 0, NULL, getXBottom(result), getXTop(result), ImVec2(0, lineH));

	ImGui::End();
	ImGui::PopStyleColor(4);

        glViewport(0, 0, WIDTH,HEIGHT);
        ImVec4 g_clear_color = ImColor(0xF1, 0xF1, 0xF1);
        glClearColor(g_clear_color.x, g_clear_color.y, g_clear_color.z, g_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window_);

}

int main()
{
	init();

#if defined(__EMSCRIPTEN__)
	emscripten_set_main_loop(main_loop, 60, 1);
#else
	while(true) {
		main_loop();
		SDL_Delay(1);
	}
#endif

	return 0;
}
