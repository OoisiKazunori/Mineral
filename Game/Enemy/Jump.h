#pragma once

class Jump
{
public:
	Jump();

	void Active();
	void Finalize();

	/// <summary>
	/// ジャンプする際のY軸の値の変動
	/// </summary>
	/// <returns>ジャンプ力</returns>
	float Update();

private:
	bool m_activeFlag;
	float m_gravity;
	float m_jumpVel;
};

