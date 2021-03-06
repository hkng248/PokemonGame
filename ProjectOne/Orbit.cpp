/**
 * \file Orbit.cpp
 *
 * \author Haitai Ng 
 */

#include "stdafx.h"
#include "Orbit.h"
#include "Blastoise.h"
#include "PokeBall.h"
#include "DoubleBufferDC.h"
#include "Emitter.h"
#include "PokestopVisitor.h"
#include "PokemonVisitor.h"
#include "string"
#include <vector>
#include <sstream>


using namespace std; 
using namespace Gdiplus;

/// Playing area width in virtual pixels
const static int Width = 1400;

/// Playing area height in virtual pixels
const static int Height = 1100;

/// Radius of the playing read in virtual pixels
const static int Radius = 500;

/// Maximum pokeball number;
const int MaxPokeballNum = 13;
/**
 * Constructor 
 */
COrbit::COrbit()
{
	/**It uses the static member function Bitmap::FromFile to load
	the image from the disk. Then it checks to ensure it loaded okay.
	If not, it displays an error message using the message function box */
	mAshImage = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/ash.png"));
	if (mAshImage->GetLastStatus() != Ok)
	{
		AfxMessageBox(L"Failed to open images/ash.png");
	}
	mItemImage = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/pokeball.png"));
	if (mItemImage->GetLastStatus() != Ok)
	{
		AfxMessageBox(L"Failed to open images/ash.png");
	}
	mEmitter = std::make_shared<CEmitter>(this);

	/// Initialize blastoise image 
	mBlastoiseImage = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/blastoise.png"));
	if (mItemImage->GetLastStatus() != Ok)
	{
		AfxMessageBox(L"Failed to open images/blastoise.png");
	}

	/// Initialize charmeleon image 
	mCharmeleonImage = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/charmeleon.png"));
	if (mItemImage->GetLastStatus() != Ok)
	{
		AfxMessageBox(L"Failed to open images/charmeleon.png");
	}

	/// Initialize pikachu image 
	mPikachuImage = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/pikachu.png"));
	if (mItemImage->GetLastStatus() != Ok)
	{
		AfxMessageBox(L"Failed to open images/pikachu.png");
	}

}


/**
 * Destructor
 */
COrbit::~COrbit()
{
}



/**
* Draw the game area & ash 
* \param graphics The GDI+ graphics context to draw on
* \param width Width of the client window
* \param height Height of the client window
*/
void COrbit::OnDraw(Gdiplus::Graphics *graphics, int width, int height)
{
	// Fill the background with black
	SolidBrush brush(Color::Black);
	graphics->FillRectangle(&brush, 0, 0, width, height);

	//
	// Automatic Scaling
	//
	float scaleX = float(width) / float(Width);
	float scaleY = float(height) / float(Height);
	float scale = min(scaleX, scaleY);
	float radius = COrbit::Radius;

	float xOffset = width / 2.0f;
	float yOffset = height / 2.0f;

	SetXOffset(width / 2.0f);
	SetYOffset(height / 2.0f);

	SetScale(scale);

	graphics->TranslateTransform(GetXOffset(), GetYOffset());
	graphics->ScaleTransform(scale, scale);

	// From here on you are drawing virtual pixels...

	//
	// And the circle so you can see how this works...
	//
	Pen pen(Color::Green);
	graphics->DrawArc(&pen, -radius, -radius, radius * 2, radius * 2, 0, 360);

	/// Get the width and height for mAshImage 
	int mAshImageHeight = (mAshImage->GetHeight()); 
	int mAshImageWidth = (mAshImage->GetWidth()); 

	/// Apply the conversion to the width and height values so they can become coordinates  
	int mAshImageY = -1 * mAshImageHeight / 2; 
	int mAshImageX = -1 * (mAshImageWidth / 2); 

	/// This will draw Ash Ketchum. Ash is located in the center of the orbit. 
	/// Shift the image a little bit. (-ashimagewidth /2, -ashimageheight/2
	graphics->DrawImage(mAshImage.get(), mAshImageX, mAshImageY,
		mAshImage->GetWidth(), mAshImage->GetHeight());

}


/**
* Draw the aquarium
* \param graphics The GDI+ graphics context to draw on
*/
void COrbit::Draw(Gdiplus::Graphics *graphics)
{
	mPokeballOffset = 0;
	//Iterate through collection and draw them. 
	for (auto item : mItems) ///< auto figures out the type automatically( try to use auto as much as possible) 
	{
		item->Draw(graphics);
	}

	// Draw pokeballs on left side of screen
	for (auto ball = 0; ball < mPokeballs; ball++)
	{
		double wid = mItemImage->GetWidth();
		double hit = mItemImage->GetHeight();
		graphics->DrawImage(mItemImage.get(),
			float(-600), float(-500+ mPokeballOffset),
			(float)mItemImage->GetWidth(), (float)mItemImage->GetHeight());
		mPokeballOffset += 75;
	}


	FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 16);

	/// Score 
	/// Draw a blastoise when a blastoise has been caught 
	if (mBlastoise > 0)
	{ 
		wstringstream str;
		str << mBlastoise << ends;  ///< convert double to string 

		/// Coordinates for the blastoise score (upper right corner) 
		graphics->DrawImage(mBlastoiseImage.get(), float(500), float(-275), 
			(float)mBlastoiseImage->GetWidth(), (float)mBlastoiseImage->GetHeight());

		/// Blastoise Brush 
		SolidBrush BlastoiseBrush(Color(255, 255, 255));
		graphics->DrawString(str.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(625, -225),   // Where to draw (top left corner)
			&BlastoiseBrush);    // The brush to draw the text with

	}

	/// Draw a charmeleon when a charmeleon has been caught 
	if (mCharmeleon > 0)
	{
		wstringstream str;
		str << mCharmeleon / 2 << ends;  ///< convert double to string 

		/// Coordinates for the charmeleon score (upper right corner) 
		graphics->DrawImage(mCharmeleonImage.get(), float(500), float(-400),
			(float)mCharmeleonImage->GetWidth(), (float)mCharmeleonImage->GetHeight());


		/// Charmeleon Brush 
		SolidBrush CharmeleonBrush(Color(255, 255, 255));
		graphics->DrawString(str.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(625, -350),   // Where to draw (top left corner)
			&CharmeleonBrush);    // The brush to draw the text with
	}

	if (mPikachu > 0)
	{
		wstringstream str;
		str << mPikachu / 3 << ends;  ///< convert double to string 

		/// these are the general coordinates of where we want to draw the image. 
		graphics->DrawImage(mPikachuImage.get(), float(500), float(-500), (float)mPikachuImage->GetWidth(), (float)mPikachuImage->GetHeight());

		/// Pikachu Brush 
		SolidBrush PikachuBrush(Color(255, 255, 255));
		graphics->DrawString(str.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(625, -450),   // Where to draw (top left corner)
			&PikachuBrush);    // The brush to draw the text with


	}
}


/**
* Add an item to the aquarium
* \param item New item to add
*/
void COrbit::Add(std::shared_ptr<CItem> item)
{
	mItems.push_back(item);
}

void COrbit::Delete(std::shared_ptr<CItem> item)
{
	auto loc = find(begin(mItems), end(mItems), item);
	if (loc != end(mItems))
	{
		mItems.erase(loc);
	}

}


/** Test an x,y click location to see if it clicked
* on some item in the aquarium.
* \param x X location
* \param y Y location
* \returns Pointer to item we clicked on or nullptr if none.
*/
std::shared_ptr<CItem> COrbit::HitTest(int x, int y)
{
	for (auto i = mItems.rbegin(); i != mItems.rend(); i++)
	{
		if ((*i)->HitTest(x, y))
		{
			return *i;
		}
	}

	return  nullptr;
}


/** Move an item to the front of the list of items.
*
* Removes item from the list and adds it to the end so it
* will display last.
* \param item The item to move
*/
void COrbit::MoveToFront(std::shared_ptr<CItem> item)
{
	auto loc = find(begin(mItems), end(mItems), item);
	if (loc != end(mItems))
	{
		mItems.erase(loc);
	}

	mItems.push_back(item);
}


/** Handle updates for animation
* \param elapsed The time since the last update
*/
void COrbit::Update(double elapsed)
{
	for (auto item : mItems)
	{
		item->Update(elapsed);
	}
	mEmitter->Update(elapsed);
}


/** Accept a visitor for the collection
* \param visitor The visitor for the collection
*/
void COrbit::Accept(CItemVisitor *visitor)
{
	for (auto item : mItems)
	{
		item->Accept(visitor);
	}
}
/** Handle the adding pokeball and changing the pokestop color.
 *	\param xclick the x location in pixels when clicking.
 *  \param yclick the y location in pixels when clicking
 */
void COrbit::Click(float xclick, float yclick)
{
	/// Adjust the x,y location base on the click point.
	mPointX = (xclick - this->GetXOffset()) * (1 / mScale);
	mPointY = (yclick - this->GetYOffset()) * (1 / mScale);
	auto item = this->HitTest(mPointX, mPointY);
	
	if (mPokeballs > 0)
	{
		auto pokeball = make_shared<CPokeBall>(this);
		pokeball->SetSpeed(mPointX, mPointY);
		

		if (sqrt(mPointX * mPointX + mPointY * mPointY) < Radius)
		{

			if (item == nullptr)
			{
				this->Add(pokeball);
				mPokeballs -= 1;

			}
			else
			{
				if (!item->ValidToAddPokeball())
				{
					this->Add(pokeball);
					mPokeballs -= 1;

				}
				else
				{
					if (item->State())
					{
						this->Add(pokeball);
						mPokeballs -= 1;

					}

				}
			}
		}
		
	}
	
	if (item != nullptr)
	{
		// We grabbed something
		// Move it to the front
		this->MoveToFront(item);
		
		// Create a visitor to change pokestop color to purple
		CPokestopVisitor visitor;
		item->Accept(&visitor);
		
	}
	
}

/**
* Add pokeballs
* \param num num to add
*/
void COrbit::AddPokeball(int num)
{
	mPokeballs += num;
	if (mPokeballs > MaxPokeballNum)
	{
		mPokeballs = MaxPokeballNum;
	}
}
/**
 * Destroy an object in the mItems 
 * \returns bool 
 */
bool COrbit::Destroyed()
{

	for (auto other : mItems) ///< iterate through collection 
	{
		/// Determine if a pokeball has hit a pokemon 
		mObject = PokemonCaught(other);

		/// If pokeball hits the green circle, destroy it, if a pokestop is 60 sec old, destroy it  
		if (other->DestroyObject() || other->NotActive()) 
		{
			toRemove.push_back(other);
		} 

		/// If the pokeball hits a pokemon, then mObject will not equal nullptr 
		else if (mObject != nullptr)
		{
			toRemove.push_back(mObject); 
			toRemove.push_back(other); 
		}

		/// If the toRemove vector is not empty 
		if (!toRemove.empty())
		{
			for (auto item : toRemove)
			{
				Delete(item);
			}
			toRemove.clear();
			
			
			return true; 
		}
	}
	return false;
}


/**
 * Determine if a pokemon has been caught. If a pokemon is hit, return a pointer to the pokemon, else return nullptr  
 * \param item This is a pointer to pokemon
 * \returns pointer This is a pointer to pokeball 
 */
std::shared_ptr<CItem> COrbit::PokemonCaught(std::shared_ptr<CItem> item)
{
	for (auto other : mItems)
	{
		///Do not compare to ourselves 
		if (other == item)
		{
			continue; 
		}

		/// If one object is a pokeball, and the other item (pokemon) is on top then return the pointer for the pokeball
		/// so it can be deleted in Destroyed() 
		if (other->ValidToCatch() && item->IsPokemon() &&
			other->HitTest((int)item->GetX(), (int)item->GetY()))
		{
			DeterminePokemonCount(item); 
			return other;
		}
	}
	return nullptr; 
}


/**
 * Determine what pokemon has been caught 
 * \param item (Pokemon) 
 * \returns number of pokemon caught 
 */
void COrbit::DeterminePokemonCount(std::shared_ptr<CItem> item)
{
	CPokemonVisitor visitor; 
	item->Accept(&visitor); 
	mBlastoise += visitor.GetBlastoiseCount(); 
	mCharmeleon += visitor.GetCharmeleonCount(); 
	mPikachu += visitor.GetPikachuCount(); 
	 
}