#include "UIElements.h"

const char* availableTypes[] = { "cube" }; 
char name_input_text[] = {};


UIElements::UIElements()
{
}

UIElements::~UIElements()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIElements::init(GLFWwindow* window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void UIElements::update_start(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIElements::update_end(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIElements::list_update(){
    ImGui::Begin("Object List");

    // Add object button
    if (ImGui::Button("Add Object")) {
        showAddObjectPopup = true;
    }

    // Add object popup
    if (showAddObjectPopup) {
        ImGui::OpenPopup("Add Object Popup");
    }

    if (ImGui::BeginPopupModal("Add Object Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Select Object Type:");
        ImGui::Combo("Type", &selectedType, availableTypes, IM_ARRAYSIZE(availableTypes));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            // Create new object
            GameObject newObject;
            int new_id = 0;
            while (true)
            {
                bool id_found = false;
                for (int i = 0; i < gameObjects.size(); i ++){
                    if (new_id == gameObjects[i].ID) id_found = true;
                }
                if (!id_found){
                    break;
                }
                else{
                    new_id++;
                }
            }
            newObject.ID = new_id;
            newObject.type = availableTypes[selectedType];
            newObject.name = "cube_" + std::to_string(gameObjects.size());  // Simple name generation

            gameObjects.push_back(newObject);
            showAddObjectPopup = false;
            ImGui::CloseCurrentPopup();
            selectedType = 0; // Reset selection
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            showAddObjectPopup = false;
            ImGui::CloseCurrentPopup();
            selectedType = 0; // Reset selection
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();

    // Object list
    ImGui::Text("Objects:");
    ImGui::BeginChild("object_list_region", ImVec2(0, 100), true); // scrollable region
    for (const auto& obj : gameObjects) {
        if(ImGui::Button(obj.name.c_str())){
            selectedObject = obj;
            showPrefs = true;
        }
    }
    ImGui::EndChild();
    ImGui::End();
}

static char nameBuffer[256] = ""; // Static buffer for name input

void UIElements::prefs_update(){
    ImGui::Begin("Object Prefs");
    if (showPrefs){
        ImGui::Text("name: ");
        ImGui::SameLine();
        ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));

        ImGui::InputFloat("X", &selectedObject.coords[0]);
        ImGui::InputFloat("Y", &selectedObject.coords[1]);
        ImGui::InputFloat("Z", &selectedObject.coords[2]);

        ImGui::Separator();

        if (ImGui::Button("Delete")) {
            for(int i = 0; i < gameObjects.size(); i ++){
                if (selectedObject.ID == gameObjects[i].ID){
                    gameObjects.erase(gameObjects.cbegin() + i);
                    showPrefs = false;
                }
            }
        }
    }
    else{
        ImGui::Text("no object selected!");
    }
    //ImGui::EndChild();
    ImGui::End();
}

std::vector<GameObject> UIElements::get_objects(){
    return gameObjects;
}

void UIElements::set_objects(std::vector<GameObject> objects){
    gameObjects = objects;
}