/**
* \ file PokeBall.h
*
* \ author Quan Zhou
* 
* Class that describe a pokeballl
*/

#pragma once
#include "LinearMovement.h"
#include "Item.h"

/**
 * Implement a pokeball
 */
class CPokeBall :
	public CLinearMovement
{
public:

	/// Constructor
	CPokeBall(COrbit * orbit);

	/// Default constructor (disabled)
	CPokeBall() = delete;

	/// Copy constructor (disabled)
	CPokeBall(const CPokeBall &) = delete;

	/// Virtual destructor 
	virtual ~CPokeBall();

	/**
	* Is it valid to catch
	* \return bool. Valid to ctach Pokemon 
	*/
	bool ValidToCatch() { return true; }
};

