// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DataBase/DGMA_TurretStruct.h"

FDGMA_TurretStruct& FDGMA_TurretStruct::operator=(const FDGMA_TurretStruct& turret_pack)
{
	Name = turret_pack.Name;
	Team = turret_pack.Team;
	Price = turret_pack.Price;
	Health = turret_pack.Health;
	Damage = turret_pack.Damage;
	Icon = turret_pack.Icon;
	MaterialInstance = turret_pack.MaterialInstance;

	return *this;
}
