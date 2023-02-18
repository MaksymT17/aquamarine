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
		base_img_path = "inputs/rs_1.bmp";
		cmp_img_path = "inputs/rs_2.bmp";
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
	amApi.compare_and_save_diff_img(base_img_path, cmp_img_path, out_img_path);

	return 0;
}