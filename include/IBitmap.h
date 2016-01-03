#ifndef IBITMAP_H_
#define IBITMAP_H_

#include <Ice/Config.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Cardroid.h"
#include "Util.h"
#include "DbTypes.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface IBitmap
 * Public operations interface for the implementation of a Bitmap object
 */
class IBitmap {

public:

	/**
	 * \enum CompressFormat
	 * The compress formats available for a IBitmap implementation
	 */
	enum CompressFormat {
		JPEG, PNG, WEBP
	};

	/**
	 * \enum Config
	 * The color configurations available for a IBitmap implementation
	 */
	enum Config {
		ALPHA8, ARGB4444, ARGB8888, RGB565
	};

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param bitmapObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IBitmap> newInstance(
			const shared_ptr<IBitmap> bitmapObject) = 0;

	/**
	 * @param bitmap The new bitmap data, in the form of a Blob, which
	 * 					might need to be read by means of a binary stream
	 */
	virtual void setBitmap(const ::oracle::occi::Blob bitmap) = 0;

	/**
	 * @return The bitmap data, in the form of a Blob, which might need to be
	 * 			read by means of a binary stream
	 */
	virtual const ::oracle::occi::Blob getBitmap() = 0;

	/**
	 * @param compressFormat The new compress format for this IBitmap
	 */
	virtual void setCompressFormat(CompressFormat compressFormat) = 0;

	/**
	 * @return The compress format for this IBitmap
	 */
	virtual CompressFormat getCompressFormat() = 0;

	/**
	 * @param config The new color configuration for this IBitmap
	 */
	virtual void setConfig(Config config) = 0;

	/**
	 * @return The color configuration for this IBitmap
	 */
	virtual Config getConfig() = 0;

	/**
	 * @param density The new image's density for this IBitmap
	 */
	virtual void setDensity(int density) = 0;

	/**
	 * @return The image's density for this IBitmap
	 */
	virtual int getDensity() = 0;

	virtual ~IBitmap();
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Bitmap
 * Domain class implementing a Bitmap for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Bitmap: private ::cardroid::data::zerocice::BitmapTyp, public IBitmap {

public:

	Bitmap() {
	}

	/**
	 * Default constructor
	 */
	Bitmap(::cardroid::data::zerocice::Blob bmBitmap,
			::cardroid::data::zerocice::CompressFormat bmCompressFormat,
			::cardroid::data::zerocice::Config bmConfig, int bmDensity) {
		bitmapBitmap = bmBitmap;
		bitmapCompressFormat = bmCompressFormat;
		bitmapConfig = bmConfig;
		density = bmDensity;
	}

	Bitmap(::cardroid::data::zerocice::BitmapTyp bitmap) {
		bitmapBitmap = bitmap.bitmapBitmap;
		bitmapCompressFormat = bitmap.bitmapCompressFormat;
		bitmapConfig = bitmap.bitmapConfig;
		density = bitmap.density;
	}

	Bitmap(const Bitmap& bitmap) {
		bitmapBitmap = bitmap.bitmapBitmap;
		bitmapCompressFormat = bitmap.bitmapCompressFormat;
		bitmapConfig = bitmap.bitmapConfig;
		density = bitmap.density;
	}

	Bitmap(Bitmap&& bitmap) {
		swap(bitmapBitmap, bitmap.bitmapBitmap);
		swap(bitmapCompressFormat, bitmap.bitmapCompressFormat);
		swap(bitmapConfig, bitmap.bitmapConfig);

	}

	~Bitmap() {
		delete &bitmapBitmap;
	}

	Bitmap& operator=(Bitmap bitmap) {
		bitmapCompressFormat = bitmap.bitmapCompressFormat;
		bitmapConfig = bitmap.bitmapConfig;
		bitmapBitmap.swap(bitmap.bitmapBitmap);
		return *this;
	}

	Bitmap& operator=(Bitmap& bitmap) {
		bitmapBitmap.swap(bitmap.bitmapBitmap);
		swap(bitmapCompressFormat, bitmap.bitmapCompressFormat);
		swap(bitmapConfig, bitmap.bitmapConfig);
		return *this;
	}

	/* IBitmap interface */

	shared_ptr<IBitmap> newInstance(const shared_ptr<IBitmap> bitmapObject) {
		if (!bitmapObject)
			return NULL;
		if (dynamic_cast< ::cardroid::data::zerocice::BitmapTyp*>(bitmapObject.get()))
			return bitmapObject;
		::cardroid::data::zerocice::Blob bitmap =
				::Cardroid::occi::blobToByteArray(bitmapObject->getBitmap());
		::cardroid::data::zerocice::CompressFormat compressFormat =
				static_cast< ::cardroid::data::zerocice::CompressFormat>(bitmapObject->getCompressFormat());
		::cardroid::data::zerocice::Config config =
				static_cast< ::cardroid::data::zerocice::Config>(bitmapObject->getConfig());
		int density = bitmapObject->getDensity();
		return static_pointer_cast<IBitmap>(
				make_shared<Bitmap>(bitmap, compressFormat, config, density));
	}

	void setBitmap(const ::oracle::occi::Blob bitmap) {
		bitmapBitmap = ::Cardroid::occi::blobToByteArray(bitmap);
	}

	const ::oracle::occi::Blob getBitmap() {
		return ::Cardroid::occi::byteArrayToBlob(bitmapBitmap);
	}

	void setCompressFormat(IBitmap::CompressFormat compressFormat) {
		bitmapCompressFormat =
				static_cast< ::cardroid::data::zerocice::CompressFormat>(compressFormat);
	}

	IBitmap::CompressFormat getCompressFormat() {
		return static_cast<IBitmap::CompressFormat>(bitmapCompressFormat);
	}

	void setConfig(IBitmap::Config config) {
		bitmapConfig = static_cast< ::cardroid::data::zerocice::Config>(config);
	}

	IBitmap::Config getConfig() {
		return static_cast<IBitmap::Config>(bitmapConfig);
	}

	void setDensity(int density_) {
		density = density_;
	}

	int getDensity() {
		return density;
	}

private:

	typedef ::cardroid::data::zerocice::BitmapTyp super;
	static const long long int serialVersionUID = -54799676768736432L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Bitmap
 * Domain class implementing a Bitmap for its storage and retrieval from an
 * Oracle database
 */
class Bitmap: private ::BitmapTyp, public IBitmap {

public:

	Bitmap() {
	}

	/**
	 * Default constructor
	 */
	Bitmap(::oracle::occi::Blob bitmap, string compressFormatName,
			string configName, int density) {
		super::setBitmap(bitmap);
		super::setCompress_format(compressFormatName);
		super::setConfig(configName);
		super::setDensity(density);
	}

	Bitmap(::BitmapTyp bitmap) {
		super::setBitmap(bitmap.getBitmap());
		super::setCompress_format(bitmap.getCompress_format());
		super::setConfig(bitmap.getConfig());
		super::setDensity(bitmap.getDensity());
	}

	Bitmap(const Bitmap& bitmap) {
		Bitmap& bm = const_cast<Bitmap&>(bitmap);
		setBitmap(bm.getBitmap());
		setCompressFormat(bm.getCompressFormat());
		setConfig(bm.getConfig());
		setDensity(bm.getDensity());
	}

	Bitmap(Bitmap&& bitmap) {
		setBitmap(bitmap.getBitmap());
		setCompressFormat(bitmap.getCompressFormat());
		setConfig(bitmap.getConfig());

	}

	~Bitmap() {
	}

	Bitmap& operator=(Bitmap bitmap) {
		setCompressFormat(bitmap.getCompressFormat());
		setConfig(bitmap.getConfig());
		setBitmap(bitmap.getBitmap());
		return *this;
	}

	Bitmap& operator=(Bitmap& bitmap) {
		setBitmap(bitmap.getBitmap());
		setCompressFormat(bitmap.getCompressFormat());
		setConfig(bitmap.getConfig());
		return *this;
	}

	/* IBitmap interface */

	shared_ptr<IBitmap> newInstance(const shared_ptr<IBitmap> bitmapObject) {
		if (!bitmapObject)
			return NULL;
		if (dynamic_cast< ::BitmapTyp*>(bitmapObject.get()))
			return bitmapObject;
		const ::oracle::occi::Blob& bitmap = bitmapObject->getBitmap();
		string compressFormatName;
		switch (bitmapObject->getCompressFormat()) {
		case (IBitmap::CompressFormat::JPEG):
			compressFormatName = "JPEG";
			break;
		case (IBitmap::CompressFormat::PNG):
			compressFormatName = "PNG";
			break;
		case (IBitmap::CompressFormat::WEBP):
			compressFormatName = "WEBP";
			break;
		}
		string configName;
		switch (bitmapObject->getConfig()) {
		case (IBitmap::Config::ALPHA8):
			configName = "ALPHA8";
			break;
		case (IBitmap::Config::ARGB4444):
			configName = "ARGB4444";
			break;
		case (IBitmap::Config::ARGB8888):
			configName = "ARGB8888";
			break;
		case (IBitmap::Config::RGB565):
			configName = "RGB565";
			break;
		}
		int density = bitmapObject->getDensity();
		return static_pointer_cast<IBitmap>(
				make_shared<Bitmap>(bitmap, compressFormatName, configName,
						density));
	}

	void setBitmap(const ::oracle::occi::Blob bitmap) {
		try {
			super::setBitmap(bitmap);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const ::oracle::occi::Blob getBitmap() {
		::oracle::occi::Blob bitmap = 0;
		try {
			bitmap = super::getBitmap();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return bitmap;
	}

	void setCompressFormat(IBitmap::CompressFormat compressFormat) {
		string compressFormatName;
		switch (compressFormat) {
		case (IBitmap::CompressFormat::JPEG):
			compressFormatName = "JPEG";
			break;
		case (IBitmap::CompressFormat::PNG):
			compressFormatName = "PNG";
			break;
		case (IBitmap::CompressFormat::WEBP):
			compressFormatName = "WEBP";
			break;
		}
		setCompress_format(compressFormatName);
	}

	IBitmap::CompressFormat getCompressFormat() {
		IBitmap::CompressFormat compressFormat;
		if (!getCompress_format().compare("JPEG"))
			return compressFormat = IBitmap::CompressFormat::JPEG;
		if (!getCompress_format().compare("PNG"))
			return compressFormat = IBitmap::CompressFormat::PNG;
		else
			return compressFormat = IBitmap::CompressFormat::WEBP;
	}

	void setConfig(IBitmap::Config config) {
		string configName;
		switch (config) {
		case (IBitmap::Config::ALPHA8):
			configName = "ALPHA8";
			break;
		case (IBitmap::Config::ARGB4444):
			configName = "ARGB4444";
			break;
		case (IBitmap::Config::ARGB8888):
			configName = "ARGB8888";
			break;
		case (IBitmap::Config::RGB565):
			configName = "RGB565";
			break;
		}
		setConfig(configName);
	}

	IBitmap::Config getConfig() {
		IBitmap::Config config = IBitmap::Config::ALPHA8;
		try {
			if (!super::getConfig().compare("ALPHA8"))
				return config = IBitmap::Config::ALPHA8;
			if (!super::getConfig().compare("ARGB4444"))
				return config = IBitmap::Config::ARGB4444;
			if (!super::getConfig().compare("ARGB8888"))
				return config = IBitmap::Config::ARGB8888;
			else
				return config = IBitmap::Config::RGB565;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return config;
	}

	void setDensity(int density) {
		setDensity(density);
	}

	int getDensity() {
		int density = 0;
		try {
			density = super::getDensity();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return density;
	}

	/* Overriding superclass */

	::oracle::occi::Blob getBitmap() const {
		::oracle::occi::Blob bitmap = 0;
		try {
			bitmap = super::getBitmap();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return bitmap;
	}

	void setCompress_format(const string& value) {
		try {
			super::setCompress_format(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getConfig() const {
		string config = "";
		try {
			config = super::getConfig();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return config;
	}

	void setConfig(const string& value) {
		try {
			super::setConfig(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getDensity() const {
		::oracle::occi::Number density;
		try {
			density = super::getDensity();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return density;
	}

	void setDensity(const ::oracle::occi::Number& value) {
		try {
			super::setDensity(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID._TYP");
	}

private:

	typedef ::BitmapTyp super;
};
}
}
}

#endif /* IBITMAP_H_ */
