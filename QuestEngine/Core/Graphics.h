#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Components/Camera.h"
#include "Events/Event.h"
#include "QuestEngine.h"
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <queue>

class Graphics
{
	friend class QuestEngine;
	Event<int> RefreshMSAASampleEvent;
	
	struct VAOData
	{
		std::unordered_map<GLFWwindow*, GLuint> m_vao;
		bool m_initialised = false;
		Mesh* m_mesh = nullptr;
	};

	std::vector<VAOData> m_sharedVao;
	std::queue<unsigned int> m_freeSharedVAO;
	std::set<GLFWwindow*> m_glfwContexts;

public:
	enum class AntiAliasingType
	{
		None,
		SSAA,
		MSAA_RT,
		MSAA,
	};

private:
	AntiAliasingType m_antiAliasingType;
	int m_MSAASample = 4;
	int m_SSAAMultiplier = 1;

	static Graphics* instance;
	Graphics();

public:
	static Graphics* GetInstance();

	void SetAntiAliasingType(AntiAliasingType antiAliasingType);
	AntiAliasingType GetAntiAliasingType();
	void Clear(CameraComponent* camera, int x, int y, int width, int height);
	void BindMainFrameBuffer();
	void SetMSAASample(int msaaSample);
	int GetMSAASample();
	void SetSSAAMultiplier(int ssaaMultiplier);
	int GetSSAAMultiplier();

	void GetVAO(int globalVAO);


	void AddGLFWContext(GLFWwindow* glfwContext, bool initialiseVAOContext = true);
	void RemoveGLFWContext(GLFWwindow* glfwContext);
	void SetupVAOContext(GLFWwindow* glfwContext, VAOData& vaoData);
	void SetupVertexAttribs(GLuint index, GLuint vboID, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

	void GenSharedVAO(Mesh* mesh, unsigned int& sharedID);
	void DeleteSharedVAO(unsigned int& sharedID);
	void SetupVAO(unsigned int sharedID);
	unsigned int GetVAO(unsigned int sharedID);
	void BindVAO(unsigned int sharedID);

	void RenderImage(Window* window, RenderTexture2D* renderTextureTarget, Shader* shader, Material* material);
};

#endif