

class ShadowMap {


public :
	 unsigned int m_frameBuffer;
	
	 std::vector<unsigned int> m_depthBuffers;
	 void Init() 
	{
		glGenFramebuffers(1, &m_frameBuffer);
		
	}

	void RenderShadowMaps(std::vector<Light> light) 
	{

		
		
	}

};