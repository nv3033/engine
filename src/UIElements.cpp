#include "UIElements.h"

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

void UIElements::top_bar_update(GLFWwindow* window){
    if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
        // Пункт меню "New" (можно добавить, если нужно)
        if (ImGui::MenuItem("New")) {
            // Логика создания нового файла
            std::cout << "New file clicked!" << std::endl;
        }

        // Пункт меню "Open" (можно добавить, если нужно)
        if (ImGui::MenuItem("Open")) {
            // Логика открытия файла
            std::cout << "Open file clicked!" << std::endl;
        }

        // Пункт меню "Save"
        if (ImGui::MenuItem("Save")) {
            // Здесь будет вызов диалога сохранения файла
            std::cout << "Save file clicked!" << std::endl;
            // TODO: Реализовать вызов диалога сохранения
        }

        // Пункт меню "Save As..." (опционально)
        if (ImGui::MenuItem("Save As...")) {
            // Логика сохранения как...
            std::cout << "Save As... clicked!" << std::endl;
            // TODO: Реализовать вызов диалога сохранения
        }

        ImGui::Separator(); // Разделитель

        // Пункт меню "Exit"
        if (ImGui::MenuItem("Exit")) {
            // Закрытие приложения
            glfwSetWindowShouldClose(window, true); // Предполагаем, что 'window' доступен
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Game")) {
        // Пункт меню "New" (можно добавить, если нужно)
        if (ImGui::MenuItem("Play")) {
            // Логика создания нового файла
            std::cout << "New file clicked!" << std::endl;
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

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
            newObject.name = "object_" + std::to_string(new_id);  // Simple name generation

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

void UIElements::prefs_update(int textures_count){
    ImGui::Begin("Object Prefs");
    if (showPrefs){
        ImGui::Text("Current Object: %s", selectedObject.name.c_str());
        ImGui::InputText("New Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));


        if(ImGui::Button("change name", ImVec2(120, 0))){
            std::string new_name = nameBuffer;
            bool name_found = false;
            for (int i = 0; i < gameObjects.size(); i ++){
                if (gameObjects[i].name == new_name){
                    name_found = true;
                }
            }
            if (!name_found){
                selectedObject.name = new_name;
                find_object_by_id(selectedObject.ID)->name = selectedObject.name;
            }
            else{
                ImGui::OpenPopup("Name Exists");
            }
        }
        if (ImGui::BeginPopupModal("Name Exists", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Name already exists!");
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();

            }
            ImGui::EndPopup();
        }


        ImGui::InputFloat("posX", &selectedObject.coords[0]);
        ImGui::InputFloat("posY", &selectedObject.coords[1]);
        ImGui::InputFloat("posZ", &selectedObject.coords[2]);

        find_object_by_id(selectedObject.ID)->coords[0] = selectedObject.coords[0];
        find_object_by_id(selectedObject.ID)->coords[1] = selectedObject.coords[1];
        find_object_by_id(selectedObject.ID)->coords[2] = selectedObject.coords[2];

        ImGui::Separator();
        
        if(selectedObject.type == "wall"){
            ImGui::InputFloat("rotX", &selectedObject.rotation[0]);
            ImGui::InputFloat("rotY", &selectedObject.rotation[1]);
            ImGui::InputFloat("rotZ", &selectedObject.rotation[2]);

            find_object_by_id(selectedObject.ID)->rotation[0] = selectedObject.rotation[0];
            find_object_by_id(selectedObject.ID)->rotation[1] = selectedObject.rotation[1];
            find_object_by_id(selectedObject.ID)->rotation[2] = selectedObject.rotation[2];
        }

        ImGui::InputInt("Texture", &selectedObject.texture_id);
        if (selectedObject.texture_id >= 0 && selectedObject.texture_id < textures_count){
            find_object_by_id(selectedObject.ID)->texture_id = selectedObject.texture_id;
        }
        else{
            selectedObject.texture_id = 0;
            find_object_by_id(selectedObject.ID)->texture_id = 0;
        }


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

GameObject* UIElements::find_object_by_id(int id){
    for(int i = 0; i < gameObjects.size(); i ++){
        if (selectedObject.ID == gameObjects[i].ID){
            return &gameObjects[i];
        }
    }
}
