#include "ImageScraper.h"
#include "Utilities.h"
#include <Windows.h>
#include <assert.h>

ImageScraper::ImageScraper(const string_t bizSiteURL)
	:m_bizPage(bizSiteURL)
{
	assert(pullBizUrlId());

	m_bizPhotosPage = buildPhotosPage();
	//if (!checkFoodFilterExistence().get())
	//{
	//	m_bizPhotosPage = buildPhotosPage(false);
	//} 
	// Above check is unnecessary. Yelp will default to main photos gallery if a food filter does not exist ~CF 10/24/2016

	m_pulledImagesLinks = new vector<string_t>;

	string_t html;
	wcout << U("Extracting photos...") << endl;
	html = extractHTML(m_bizPhotosPage).get();
	// ISSUE: Hang-ups occur with slow connection!
	// Q: How to time-out connection and continue with program?
	//std::wcout << html << std::endl;
	wcout << U("Photos extracted!") << endl;

	scrapeImageLinks(html); // Store URL path to each photo in gallery in m_pulledImagesLinks

	// TEST:
	//ShellExecute(NULL, U("open"), m_bizPhotosPage.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

ImageScraper::~ImageScraper()
{
	delete m_pulledImagesLinks;
}

pplx::task<void> ImageScraper::displayRandomPhoto() const
// TODO: This code is very inefficient! Need to refactor ~CF 10/23/2016
// TODO: Allow display of another image upon user response ~CF 10/24/2016
{
	// Use if pulling multiple photos:
	//vector<size_t> randomized = randomizeArray(m_pulledImagesLinks->size());

	// Use if only pulling one photo:
	// Only use top 6 images ~CF 10/24/2016
	size_t randomized;
	if (6 > m_pulledImagesLinks->size())
		randomized = rand() % m_pulledImagesLinks->size();
	else randomized = rand() % 6;

	const string_t host = U("https://www.yelp.com");
	const string_t path = *(m_pulledImagesLinks->begin() + randomized);
	const string_t link = host + path;

	// TEST
	//wcout << link << endl;
	//system("pause");
	// END_TEST

	web::http::client::http_client client(link);

	return client.request(web::http::methods::GET).then([&](web::http::http_response response)
	{
		try
		{
			if (response.status_code() != web::http::status_codes::OK)
			{
				wcout << response.status_code() << U(" - ") << response.reason_phrase() << endl;
				return;
			}

			wcout << U("Downloading Yelp site...") << endl;
			const string_t strResponse = response.extract_string().get(); // HTML as string
			// NOTE: This is the slowest part of the program ~CF 10/23/2016
			// TEST: Check length of html response
			//wcout << U("length: ") << strResponse.length() << endl;
			//system("pause");
			// END_TEST: delay is d/t a larger response
			wcout << U("Website downloaded!") << endl;

			// TEST: check to see why .extract_string() is slow in above code
			//wcout << U("Downloading Yelp site... again") << endl;
			//string_t response2 = response.extract_string().get();
			//wcout << U("Website downloaded!") << endl;
			//END_TEST: response already downloaded from above code, no slow-down as seen above

			string_t imageLink;
			const string_t classFilter = U("pinterest-wrapper");
			auto htmlIter = strResponse.begin();

			if (findString(htmlIter, strResponse, classFilter))
			{
				htmlIter = std::next(htmlIter, classFilter.length());
				const string_t imgTag = U("src=\"");
				if (findString(htmlIter, strResponse, imgTag))
				{
					htmlIter = std::next(htmlIter, imgTag.length());
					while (U('\"') != *htmlIter)
					{
						imageLink += *htmlIter++;
					}
					std::wcout << U("Image: ") << imageLink << std::endl;

					ShellExecute(NULL, U("open"), imageLink.c_str(), NULL, NULL, SW_SHOWNORMAL);
					return;
				};
			}

			std::wcout << U("ERROR - No image found") << std::endl;
			return;
		}
		catch (...)
		{
			std::wcout << U("ERROR - Unable to obtain selected image") << std::endl;
			return;
		}
	});
}

// Get business ID contained in the Yelp page's URL.
// Format: "host.address" "/biz/" "business-id" "?" "parameters"
// ex: https://www.yelp.com/biz/undergrind-cafe-los-angeles?adjust_creative=f3aK-mb5wKb3xr3citZw5w&utm_campaign=yelp_api_v3&utm_medium=api_v3_business_search&utm_source=f3aK-mb5wKb3xr3citZw5w
bool ImageScraper::pullBizUrlId()
{
	const string_t bizPath = U("/biz/");

	string_t::const_iterator bizPageIter = findString(m_bizPage, bizPath);
	if (m_bizPage.end() == bizPageIter)
		return false;

	for (size_t count = 0; count < bizPath.length(); count++)
	{
		bizPageIter++;
	}

	m_bizUrlId.clear();
	while (*bizPageIter != U('?') && bizPageIter != m_bizPage.end())
	{
		m_bizUrlId += *bizPageIter;
		bizPageIter++;
	}

	// TEST:
	std::wcout << U("Business URL string: ") << m_bizUrlId << std::endl;

	return true;

	// OUTDATED_CODE ~CF 10/18/2016
	//const size_t pathLength = bizPath.length(); // 5

	//unsigned int bizUrlIdStartEle = 0;

	//string_t curPath;

	//for (size_t curEle = 0; curEle <= m_bizPage.length() - pathLength; curEle++)
	//{
	//	curPath.clear();
	//	for (size_t curPathEle = curEle; curPathEle < curEle + pathLength; curPathEle++)
	//	{
	//		curPath += m_bizPage[curPathEle];
	//	}

	//	if (bizPath == curPath)
	//	{
	//		bizUrlIdStartEle = curEle + pathLength;
	//		m_bizUrlId.clear();

	//		for (size_t urlEle = bizUrlIdStartEle; urlEle < m_bizPage.length(); urlEle++)
	//		{
	//			//if (!isAlphanumerical(m_bizPage[urlEle]) && U('-') != m_bizPage[urlEle]) // TODO: Does this acount for all cases? Some restaurant IDs contain special characters!
	//			if (U('?') == m_bizPage[urlEle]) // NOTE: Need to make sure this works for all cases!
	//				break;

	//			m_bizUrlId += m_bizPage[urlEle];
	//		}

	//		// TEST:
	//		std::wcout << U("Business URL string: ") << m_bizUrlId << std::endl;

	//		return true;
	//	}
	//}

	//// Unable to find /biz/ in URL
	//return false;
	// END_OUTDATED_CODE ~CF 10/18/2016
}

// Build URL for the business photos page
// Some business will have a food filter, others will not <-- NOTE: actually unnecessary - defaults to main photos library if ?tab=food specified and non-existent ~CF 10/16/2016
// Format: "https://www.yelp.com/biz_photos/biz-id"
string_t ImageScraper::buildPhotosPage(bool foodFilter) // default: foodFilter = true
{
	string_t photosPageURL = U("https://www.yelp.com/biz_photos/");
	photosPageURL += m_bizUrlId;

	if (foodFilter)
	{
		photosPageURL += U("?tab=food");
	}

	// TEST:
	std::wcout << U("Photos URL: ") << photosPageURL << std::endl;

	return photosPageURL;
}

// Check if business Yelp page has a food filter in their photos library
pplx::task<bool> ImageScraper::checkFoodFilterExistence()
{
	web::http::client::http_client client(m_bizPhotosPage);

	return client.request(web::http::methods::GET).then([&](web::http::http_response response)
	{
		try
		{
			if (response.status_code() != web::http::status_codes::OK)
			{
				std::wcout << U("NOTE: No food filter exists in photos library") << std::endl;
				return false;
			}
			return true;
		}
		catch (...)
		{
			std::wcout << U("ERROR: Unable to connect to business photos page") << std::endl;
			return false;
		}
	});
}

pplx::task<string_t> ImageScraper::extractHTML(const string_t link) const
{
	web::http::client::http_client client(link);

	return client.request(web::http::methods::GET).then([&](web::http::http_response response)
	{
		try
		{
			if (response.status_code() != web::http::status_codes::OK)
			{
				string_t error;
				error = response.status_code();
				error += U(" - ");
				error += response.reason_phrase();
				return error;
			}

			const string_t strResponse = response.extract_string().get(); // Q: Why do I need .get() here?

			// TEST
			//wcout << U("length: ") << strResponse.length() << endl;
			//system("pause");
			// END_TEST
			// TEST
			//for (auto printThis = strResponse.begin(); printThis < strResponse.begin()+15; printThis++)
			//	std::wcout << *printThis;
			//std::wcout << endl;

			//ShellExecute(NULL, U("open"), link.c_str(), NULL, NULL, SW_SHOWNORMAL); // ERROR!
			// END_TEST

			return strResponse; // returns HTML as a string
		}
		catch(...)
		{
			std::wcout << U("ERROR - Unable to extract HTML") << std::endl;
		}
		});
}

// Get image links
void ImageScraper::scrapeImageLinks(const string_t html)
{
	m_pulledImagesLinks->clear();
	const string_t imageHTMLTag = U("href=\"");

	// TEST
	//std::wcout << U("Searching for: ") << imageHTMLTag << std::endl;
	//system("pause");
	// END_TEST

	for (string_t::const_iterator htmlIter = html.begin(); htmlIter != html.end();)
	{
		// Filter gallery photos
		const string_t galleryPhotos = U("media-landing_gallery photos");
		findString(htmlIter, html, galleryPhotos);
		if (html.end() == htmlIter)
		{
			wcout << U("No photo gallery found") << endl;
			break;
		}

		const string_t photoLabel = U("<img"); // Some photo galleries include videos. This tag filters photos ~CF 10/23/2016
		//while (findString(htmlIter++, html, imageHTMLTag)) <-- Q: Why won't this work?! ~CF 10/18/2016
		while (findString(htmlIter, html, photoLabel, U("</ul>"))) // Photos grid is displayed in an unordered list and encased within tags <ul>...</ul>
			// TODO: Brute force method, prioritize refactoring ~CF 10/24/2016
		{
			htmlIter = std::next(htmlIter, photoLabel.length()); // Saves some unnecessary iterations (probably insigifcant) ~CF 10/23/2016
			if (findString(htmlIter, html, imageHTMLTag))
			{
				string_t::const_iterator imageStrIter = htmlIter + imageHTMLTag.length(); // imageStrIter will start at the beginning of the image object
				string_t image;
				while (*imageStrIter != U('\"'))
				{
					image += *imageStrIter++;
				}
				//std::wcout << image << std::endl; // TEST
				m_pulledImagesLinks->push_back(image);

				// Only pull the top 6 images
				if (6 == m_pulledImagesLinks->size())
					break;

				htmlIter = imageStrIter;
			}
		}
		break;
	}
	//wcout << U("# of pictures: ") << m_pulledImagesLinks->size() << endl; // TEST ~CF 10/24/2016

	if (m_pulledImagesLinks->empty())
		std::wcout << U("No images found") << std::endl;

	// TEST
	//int count = 1;
	//for (auto vecIter = m_pulledImagesLinks->begin(); vecIter != m_pulledImagesLinks->end(); vecIter++)
	//{
	//	std::wcout << count++ << U(". ") << *vecIter << std::endl;
	//}
	// END_TEST
}