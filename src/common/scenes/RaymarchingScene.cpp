//
// Created by pc on 02.08.22.
//

#include "RaymarchingScene.h"
#include "common/primitives/FullscreenRectangle.h"
using namespace std;
using namespace glm;

namespace
{
    ivec3 nParticles(1000, 100, 100);

    int totalParticles = nParticles.x * nParticles.y * nParticles.z;
    GLuint particlesVao;
    GLuint bhBuf;
    GLuint bhVao;
    vec4 bh1(5, 0, 0, 1), bh2(-5, 0, 0, 1);
}

void RaymarchingScene::update(float dt)
{
    if (time == 0.0f)
    {
        deltaT = 0.0f;
    }
    else
    {
        deltaT = dt - time;
    }
    time = dt;
    if (true)
    {
        angle += speed * deltaT;
        if (angle > 360.0f) angle -= 360.0f;
    }
 
}

void RaymarchingScene::render()
{
    // Rotate the attractors ("black holes")
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 0, 1));
    glm::vec3 att1 = glm::vec3(rot * bh1);
    glm::vec3 att2 = glm::vec3(rot * bh2);

    // Execute the compute shader
    compute_shader->use();
    compute_shader->setVec3("BlackHolePos1", att1);
    compute_shader->setVec3("BlackHolePos2", att2);

    glDispatchCompute(totalParticles / 1000, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Draw the scene
    shader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = glm::lookAt(glm::vec3(2, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    mat4 model = glm::mat4(1.0f);
    mat4 projection = glm::perspective(glm::radians(50.0f), (float)800 / 600, 1.0f, 100.0f);
    glm::mat4 mv = view * model;
    glm::mat3 norm = glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));

    shader->setMat4("ModelViewMatrix", mv);
    shader->setMat4("NormalMatrix", norm);
    shader->setMat4("MVP", projection * mv);

    // Draw the particles
    glPointSize(2.0f);
    shader->setVec4("Color", glm::vec4(0, 0, 0, 0.2f));
    glBindVertexArray(particlesVao);
    glDrawArrays(GL_POINTS, 0, totalParticles);
    glBindVertexArray(0);

    // Draw the attractors
    glPointSize(10.0f);
    GLfloat data[] = { att1.x, att1.y, att1.z, 1.0f, att2.x, att2.y, att2.z, 1.0f };
    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), data);
    shader->setVec4("Color", glm::vec4(1,0, 0, 1.0f));
    glBindVertexArray(bhVao);
    glDrawArrays(GL_POINTS, 0, 2);
    glBindVertexArray(0);
}

RaymarchingScene::RaymarchingScene(CameraBase* camera) : Scene(camera), time(0.0f), deltaT(0.0f), speed(35.0f), angle(0.0f)
{
    shader = new Shader(ROOT_DIR "src/shaders/raymarching.vert", ROOT_DIR "src/shaders/raymarching.frag");

    compute_shader = new ComputeShader(ROOT_DIR "src/shaders/raymarching.comp");
    FullscreenRectangle* rect = new FullscreenRectangle();
    rect->setShader(shader);
    add(rect);

    vector<GLfloat> initPos;
    vector<GLfloat> initVel(totalParticles * 4, 0.0f);
    glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat dx = 2.0f / (nParticles.x - 1),
        dy = 2.0f / (nParticles.y - 1),
        dz = 2.0f / (nParticles.z - 1);
    // We want to center the particles at (0,0,0)
    glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -1, -1));
    for (int i = 0; i < nParticles.x; i++)
    {
        for (int j = 0; j < nParticles.y; j++)
        {
            for (int k = 0; k < nParticles.z; k++)
            {
                p.x = dx * i;
                p.y = dy * j;
                p.z = dz * k;
                p.w = 1.0f;
                p = transf * p;
                initPos.push_back(p.x);
                initPos.push_back(p.y);
                initPos.push_back(p.z);
                initPos.push_back(p.w);
            }
        }
    }

    GLuint bufs[2];
    glGenBuffers(2, bufs);
    GLuint posBuf = bufs[0];
    GLuint velBuf = bufs[1];

    GLuint bufSize = totalParticles * 4 * sizeof(GLfloat);

    // The buffers for positions
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initPos[0], GL_DYNAMIC_DRAW);

    // Velocities
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initVel[0], GL_DYNAMIC_DRAW);

    // Set up the VAO
    glGenVertexArrays(1, &particlesVao);
    glBindVertexArray(particlesVao);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set up a buffer and a VAO for drawing the attractors (the "black holes")
    glGenBuffers(1, &bhBuf);
    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    GLfloat data[] = { bh1.x, bh1.y, bh1.z, bh1.w, bh2.x, bh2.y, bh2.z, bh2.w };
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &bhVao);
    glBindVertexArray(bhVao);

    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
