#pragma once

#include "Entity.h"
#include "Mat.h"
#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

using namespace EngineTypes::NetPoint;

class NetPoint : public DynamicEntity
{
private:
	id_t connected_points_array_length;
protected:
	GameTypes::tic_t exist_time;
	NetPoint** connected_points_ps;//connected points pointers
	id_t connected_points_count;

	GameTypes::players_count_t host_number;
	GameTypes::players_count_t host_team_number;

	void AddPoint(NetPoint* connected_point);
	void RemovePoint(id_t point_id);
	void RemovePoint(NetPoint* point_p);
	void ClearMemory();
	void DesegnemtationMemory();
public:

	NetPoint();
	NetPoint(const NetPoint& net_point);
	NetPoint(
		Vec2F position,
		Vec2F velocity,
		GameTypes::tic_t exist_time,
		GameTypes::players_count_t host_number,
		GameTypes::players_count_t host_team_number,
		NetPoint** connected_points_pointers,
		id_t connected_points_count);
	NetPoint(
		const Vec2F* position,
		const Vec2F* velocity,
		GameTypes::tic_t exist_time,
		GameTypes::players_count_t host_number,
		GameTypes::players_count_t host_team_number,
		NetPoint** connected_points_pointers,
		id_t connected_points_count);
	NetPoint(
		Vec2F position,
		Vec2F velocity,
		GameTypes::tic_t exist_time,
		GameTypes::players_count_t host_number,
		GameTypes::players_count_t host_team_number,
		NetPoint* connected_points_array,
		const id_t* connected_points_ids_in_array,
		id_t ids_count);
	NetPoint(
		const Vec2F* position,
		const Vec2F* velocity,
		GameTypes::tic_t exist_time,
		GameTypes::players_count_t host_number,
		GameTypes::players_count_t host_team_number,
		NetPoint* connected_points_array,
		const id_t* connected_points_ids_in_array,
		id_t ids_count);

	void ConnectTo(NetPoint* connected_point);
	void ConnectTo(
		NetPoint* connected_points_array,
		id_t points_count);
	void ConnectTo(
		NetPoint* connected_points_array,
		const id_t* connected_points_ids_in_array,
		id_t ids_count);

	void DisConnetc(NetPoint* connected_point);
	void DisConnetc(
		NetPoint* connected_points_array,
		id_t points_count);
	void DisConnetc(
		NetPoint* connected_points_array,
		const id_t* connected_points_ids_in_array,
		id_t ids_count);
	void DisConnectAll();

	NetPoint GetConnectedPoint(id_t point_id);
	NetPoint* GetConnectedPointP(id_t point_id);
	NetPoint* GetConnectedPoints();
	NetPoint** GetConnectedPointsPs();

	bool IsConnected(const NetPoint* connected_point);

	void UpdateConnections();

	void operator=(NetPoint net_point);

	~NetPoint();
};