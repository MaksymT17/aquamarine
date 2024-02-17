#include <memory>
#include <stdio.h>

#include "analyze/algorithm/ObjectDetector.h"
#include "AmApi.h"

int main(int argc, char *argv[])
{
	std::string base_img_path, cmp_img_path;
	const char *conf_path;
	const char *out_img_path;

	// read arguments or use default for demo purposes
	if (argc == 5)
	{
		std::cout << "Parse user-defined parameters." << std::endl;
		base_img_path = std::string(argv[1]);
		cmp_img_path = std::string(argv[2]);
		conf_path = argv[3];
		out_img_path = argv[4];
	}
	else
	{
		std::cout << "Use default parameters" << std::endl;
		base_img_path = "inputs/_DSC4097.JPG";
		cmp_img_path = "inputs/_DSC4098.JPG";
		conf_path = "inputs/configuration.csv";
		out_img_path = "compare_result.bmp";
	}

	am::AmApi amApi(conf_path);

	// just for demo purposes all found rects data can be printed
	/*const auto rects = amApi.compare(base_img_path, cmp_img_path);
	for (auto &rect : rects)
	{
		printf("row:%zd col:%zd    row:%zd col:%zd value:%zd\n",
			   rect.getMinHeight(), rect.getLeft(), rect.getMaxHeight(),
			   rect.getRight(), rect.getPixelsCount());
	}*/
	amApi.enable_database_reports("results.db");
	const auto rects = amApi.compare(base_img_path, cmp_img_path);
	for (auto &rect : rects)
	{
		printf("row:%zd col:%zd    row:%zd col:%zd value:%zd\n",
			   rect.getMinHeight(), rect.getLeft(), rect.getMaxHeight(),
			   rect.getRight(), rect.getPixelsCount());
	}
	printf("rects %zu\n", rects.size());
	return 0;
}