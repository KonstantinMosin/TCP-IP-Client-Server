@set file_name=protobuf
@set mypath=%cd%
@if exist %file_name%.proto ( 
	%mypath%\protoc.exe --cpp_out=. %file_name%.proto
	@echo %file_name%.pb.cc and %file_name%.pb.h created)
@pause