#pragma once

#include "cpprest\details\basic_types.h"
#include "cpprest\http_client.h"
#include <vector>

using namespace utility;

class ImageScraper
{
public:
	ImageScraper(const string_t bizSiteURL);
	~ImageScraper();
	pplx::task<void> displayRandomPhoto() const;
private:
	const string_t BIZ_PHOTOS_URL = U("https://www.yelp.com/biz_photos/");

	string_t m_bizPage;
	string_t m_bizUrlId;
	string_t m_bizPhotosPage;
	std::vector<string_t>* m_pulledImagesLinks;

	bool pullBizUrlId();
	string_t buildPhotosPage(bool foodFilter = true);

	pplx::task<bool> checkFoodFilterExistence();
	pplx::task<string_t> extractHTML(const string_t link) const;
	void scrapeImageLinks(const string_t html);
};