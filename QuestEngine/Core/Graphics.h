#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Components/Camera.h"
#include "Events/Event.h"
#include "QuestEngine.h"
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <queue>
#include "Assets/RenderTexture2D.h"

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

	struct ClearParams {
		GLbitfield clearBufferMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT; // ce qu'on clear
		// Couleur
		float r = 0.f, g = 0.f, b = 0.f, a = 0.f;
		// Profondeur
#if defined(GL_ES_VERSION_3_0) || defined(GL_ES_VERSION_2_0)
		float depth = 1.f; // glClearDepthf en GLES
#else
		GLclampd depth = 1.0;
#endif
		// Stencil
		GLint stencilValue = 0;
		GLuint stencilWriteMask = 0xFFu;

		// Mask d'écriture
		GLboolean depthWrite = GL_TRUE;
		GLboolean colorMaskR = GL_TRUE, colorMaskG = GL_TRUE, colorMaskB = GL_TRUE, colorMaskA = GL_TRUE;
	};

private:
	AntiAliasingType m_antiAliasingType = AntiAliasingType::MSAA;
	int m_MSAASample = 32;
	int m_SSAAMultiplier = 1;

	static Graphics* instance;
	Graphics();

public:
	static Graphics* GetInstance();

	void SetAntiAliasingType(AntiAliasingType antiAliasingType);
	AntiAliasingType GetAntiAliasingType();
	void Clear(CameraComponent* camera, int x, int y, int width, int height);
	void Clear(int x, int y, int width, int height, const ClearParams& p);
	void Clear(int x, int y, int width, int height) {
		ClearParams def;
		Clear(x, y, width, height, def);
	}
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


// --- Helpers de concaténation ---
#define QE_PP_CAT_(a,b) a##b
#define QE_PP_CAT(a,b)  QE_PP_CAT_(a,b)

// --- RAII group ---
struct GLDebugGroup {
	GLDebugGroup(const char* name) { glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name); }
	~GLDebugGroup() { glPopDebugGroup(); }
};

#ifndef NDEBUG
// Un nom unique par appel grâce à __LINE__
#define GL_SCOPE(name) \
      GLDebugGroup QE_PP_CAT(__gl_scope_, __LINE__)(name)

#define GL_SCOPE_FMT(fmt, ...) do { \
      char QE_PP_CAT(__buf_, __LINE__)[256]; \
      std::snprintf(QE_PP_CAT(__buf_, __LINE__), sizeof(QE_PP_CAT(__buf_, __LINE__)), fmt, __VA_ARGS__); \
      GLDebugGroup QE_PP_CAT(__gl_scope_fmt_, __LINE__)(QE_PP_CAT(__buf_, __LINE__)); \
  } while(0)

#define GL_MARKER(msg) glDebugMessageInsert( \
      GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, \
      GL_DEBUG_SEVERITY_NOTIFICATION, -1, msg)
#else
#define GL_SCOPE(name)         ((void)0)
#define GL_SCOPE_FMT(...)      ((void)0)
#define GL_MARKER(msg)         ((void)0)
#endif

#endif

