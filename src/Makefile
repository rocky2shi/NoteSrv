
# 执行编释前的处理 [Rocky 2009-10-21 14:53:45]
$(shell sh format.sh >/dev/null 2>&1)

## 定义目标搜索路径
#vpath %.cpp Connect


#
# 定义伪目录，作用是如当前目录下有同名文件时能也正确处理（否则将以文件为准）
#
.PHONY: all



EXE_NOTESRV=NoteSrv
EXE_TEST=test
EXE_TOOLS=filename2url


TARGETS=$(EXE_NOTESRV) $(EXE_TOOLS) $(EXE_TEST)


sub_dir_file=$(patsubst %.cpp, %.o, $(wildcard */*/*.cpp))
sub_dir_file+=$(patsubst %.cpp, %.o, $(wildcard */*.cpp))


# 过滤掉不需要的部分
OBJ_MAIN = $(filter-out Page/%, $(sub_dir_file))


# 动态库对象
OBJ_DLL=$(patsubst %.cpp, %.o, $(wildcard Page/*.cpp))
EXE_DLL=$(patsubst %.o, %.so, $(OBJ_DLL))



#
# $(patsubst %.cpp, %.o, $(wildcard Table_*.cpp)) : 取当前目录下以Table_为
# 前缀的.cpp文件，然后把文件反缀.cpp转为.o作为目标文件；
#
SOBJS_PUBLIC=   $(OBJ_MAIN)                                                    \
                Common.o                                                       \
				Debug.o                                                        \
				Environment.o                                                  \
				Log.o                                                          \
				Help.o                                                         \
                RingQueueOfInt.o                                               \
                TaskQueue.o                                                    \
				SignalDeal.o                                                   \
				Counter.o                                                      \
				Timer.o                                                        \
				CondLock.o                                                     \
				Compress.o                                                     \
				ChineseCoding.o                                                \
				Encrypt.o                                                      \
				MultiStringMatch.o                                             \
				Ini.o                                                          \
				Conf.o                                                         \
				PageCfg.o                                                      \
				DataPack.o                                                     \
				$(patsubst %.cpp, %.o, $(wildcard DllLoad_*.cpp))              \
				DllLoad.o                                                      \
				GetFileList.o                                                  \
				ThreadPool.o                                                   \
				Other.o                                                        \
				Tag.o                                                          \
				Module.o                                                       \
				Connect.o                                                      \
				ConnectQueue.o                                                 \
				Page.o                                                         \
                FileObj.o                                                      \
                Request.o                                                      \
                UserData.o                                                     \
                GlobalConfig.o                                                 \
                User.o                                                         \
                Menu.o                                                         \
                Session.o                                                      \
                Submit.o                                                       \
                $(OBJ_DLL)                                                     \
                ClassInit.o                                                    \





BOOST_INC=-I ~/arm/include/boost
BOOST_LIB=-L ~/arm/lib/boost
ZIP_INC=-I ~/arm/include
ZIP_LIB=-L ~/arm/lib



#
# $(DEBUG) 调试环境变量，设为：export DEBUG=-D_DEBUG
#
CPPFLAGS:= -g -I. $(CPPFLAGS) $(DEBUG) $(BOOST_INC) $(ZIP_INC)
LIBS:= -lboost_thread -ldl -lz $(BOOST_LIB) $(ZIP_LIB)




# 最终（终极）目标
all:  exe #dll


#
# 暂取消生成动态动方式，注意，当需要启用时，同步去掉上
# 面SOBJS_PUBLIC中的$(OBJ_DLL)，以免重复；[Rocky 2010-06-04 17:00:16]
#
exe: $(TARGETS)
dll: $(EXE_DLL)









# 可执行序程
$(TARGETS): % : $(SOBJS_PUBLIC) %.o
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@
	@echo --------------------------$@ OK--------------------------
	@echo



# 动态库
$(EXE_DLL): %.so : $(SOBJS_PUBLIC) %.o
	$(CXX) $(LDFLAGS) $(LIBS) -shared -fPIC $^ -o $@
	@echo --------------------------$@ OK--------------------------
	@echo














clean:
	$(RM) *.o $(SOBJS_PUBLIC) $(TARGETS) Changes.h $(OBJ_DLL) $(EXE_DLL)
	$(RM) core core.*
	$(RM) NoteSrv.*.tar

