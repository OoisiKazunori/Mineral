#pragma once

class Jump
{
public:
	Jump();

	void Active();
	void Finalize();

	/// <summary>
	/// �W�����v����ۂ�Y���̒l�̕ϓ�
	/// </summary>
	/// <returns>�W�����v��</returns>
	float Update();

private:
	bool m_activeFlag;
	float m_gravity;
	float m_jumpVel;
};

