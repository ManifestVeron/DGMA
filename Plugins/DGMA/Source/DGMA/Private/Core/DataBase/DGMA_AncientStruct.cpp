﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Core\DataBase\DGMA_AncientStruct.h"

FDGMA_AncientStruct& FDGMA_AncientStruct::operator=(const FDGMA_AncientStruct& turret_pack)
{
	Team = turret_pack.Team;
	Health = turret_pack.Health;
	MI_Main = turret_pack.MI_Main;
	MI_HalfHP = turret_pack.MI_HalfHP;

	return *this;
}