
# ִ�б���ǰ�Ĵ��� [Rocky 2009-10-21 14:53:45]
$(shell sh format.sh >/dev/null 2>&1)

## ����Ŀ������·��
#vpath %.cpp Connect


#
# ����αĿ¼���������統ǰĿ¼����ͬ���ļ�ʱ��Ҳ��ȷ�������������ļ�Ϊ׼��
#
.PHONY: all



EXE_NOTESRV=NoteSrv
EXE_TEST=test
EXE_TOOLS=filename2url


TARGETS=$(EXE_NOTESRV) $(EXE_TOOLS) $(EXE_TEST)


sub_dir_file=$(patsubst %.cpp, %.o, $(wildcard */*/*.cpp))
sub_dir_file+=$(patsubst %.cpp, %.o, $(wildcard */*.cpp))


# ���˵�����Ҫ�Ĳ���
OBJ_MAIN = $(filter-out Page/%, $(sub_dir_file))


# ��̬�����
OBJ_DLL=$(patsubst %.cpp, %.o, $(wildcard Page/*.cpp))
EXE_DLL=$(patsubst %.o, %.so, $(OBJ_DLL))



#
# $(patsubst %.cpp, %.o, $(wildcard Table_*.cpp)) : ȡ��ǰĿ¼����Table_Ϊ
# ǰ׺��.cpp�ļ���Ȼ����ļ���׺.cppתΪ.o��ΪĿ���ļ���
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
# $(DEBUG) ���Ի�����������Ϊ��export DEBUG=-D_DEBUG
#
CPPFLAGS:= -g -I. $(CPPFLAGS) $(DEBUG) $(BOOST_INC) $(ZIP_INC)
LIBS:= -lboost_thread -ldl -lz $(BOOST_LIB) $(ZIP_LIB)




# ���գ��ռ���Ŀ��
all:  exe #dll


#
# ��ȡ�����ɶ�̬����ʽ��ע�⣬����Ҫ����ʱ��ͬ��ȥ����
# ��SOBJS_PUBLIC�е�$(OBJ_DLL)�������ظ���[Rocky 2010-06-04 17:00:16]
#
exe: $(TARGETS)
dll: $(EXE_DLL)









# ��ִ�����
$(TARGETS): % : $(SOBJS_PUBLIC) %.o
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@
	@echo --------------------------$@ OK--------------------------
	@echo



# ��̬��
$(EXE_DLL): %.so : $(SOBJS_PUBLIC) %.o
	$(CXX) $(LDFLAGS) $(LIBS) -shared -fPIC $^ -o $@
	@echo --------------------------$@ OK--------------------------
	@echo














clean:
	$(RM) *.o $(SOBJS_PUBLIC) $(TARGETS) Changes.h $(OBJ_DLL) $(EXE_DLL)
	$(RM) core core.*
	$(RM) NoteSrv.*.tar
