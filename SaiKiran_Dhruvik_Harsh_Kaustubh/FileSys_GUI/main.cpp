#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "file_operations.h"
#include <vector>
#include <sys/stat.h>
#include <sys/statvfs.h>


// Variables for user input
char dirName[64] = "";
char fileName[64] = "";
char filePath[64] = "";
std::string fileContent(1024, '\0'); // Preallocate space for 1024 characters
char writingToFile[256] = "File not Selected"; // Holds the latest status message
char statusMessage[256] = ""; // Holds the latest status message
char statusMessage0[256] = "Welcome to the Operating Systems - UCONN 5305!"; // Holds the latest status message
// char diskUsage[256] = "";
std::string diskUsage;
char statusMessage2[256] = "Process Scheduler!"; // Holds the latest status message
std::string dirContents;

char oldName[64] = "";
char newName[64] = "";
char moveCopySource[64] = "";
char moveCopyDestination[64] = "";
char copySource[64] = "";
char copyDestination[64] = "";
// Declare a string to hold the mode as a string for display
char newPermissionsStr[16];  // Buffer to hold permission string (e.g., "0644")
mode_t newPermissions = 0644; // Default permissions
int result = 0;

// std::vector<Process> processes;
int quantum = 2;
std::string scheduleOutput;


int main() {
    snprintf(newPermissionsStr, sizeof(newPermissionsStr), "%o", newPermissions);
    // Setup GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Operating Sytems - UCONN CSE 5305", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader (glad, glew, etc.) before initializing Dear ImGui
    // Example for GLAD: if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

    // Initialize Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

  // Load and scale default font
    ImFont* font = io.Fonts->AddFontDefault();

    if (font) {
        font->Scale = 1.5f;  // Scale font by 1.5x
    }

    ImFont* largeFont = io.Fonts->AddFontDefault();
    largeFont->Scale = 2.0f; // Scale default font to be larger

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");



    // Some Customization - May be we can set below dynamically on front end ?
    // ImGui::StyleColorsLight();   // Light theme
    // ImGui::StyleColorsDark();    // Dark theme
    ImGui::StyleColorsClassic(); // Classic theme


    ImGuiStyle& style = ImGui::GetStyle();

    // Adjust rounding for buttons
    style.FrameRounding = 5.0f;

    // Adjust spacing between widgets
    style.ItemSpacing = ImVec2(10, 5);

    // Adjust window padding
    style.WindowPadding = ImVec2(15, 15);



    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // adding Docking space
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

        // GUI for file system operations
        ImGui::Begin("File System Operations");

        // Display status message
        // ImGui::TextWrapped(statusMessage);
        ImGui::PushFont(largeFont);
        float windowWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(statusMessage0).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("%s", statusMessage0);
        ImGui::PopFont();

        ImGui::BeginChildFrame(ImGui::GetID("Processes"), ImVec2(0, 200), 0);
        ImGui::BeginChild("##Processes", ImVec2(ImGui::GetWindowWidth() * 0.60f, ImGui::GetTextLineHeight() * 16), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::SeparatorText("Status Message: ");
        if(result == 0){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green color
            ImGui::Text("%s", statusMessage);
            ImGui::PopStyleColor();
        } else{
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red color
            ImGui::Text("%s", statusMessage);
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();        
        ImGui::SameLine();
        ImGui::BeginChild("##Processes2", ImVec2(ImGui::GetWindowWidth() * 0.40f, ImGui::GetTextLineHeight() * 16), true);
        ImGui::SeparatorText("Disk Usage: ");
        struct statvfs diskInfo = get_disk_usage(".");
        diskUsage.clear();
        diskUsage.append("Total space: ");
        diskUsage.append(std::to_string((diskInfo.f_blocks * diskInfo.f_frsize) / (1024 * 1024)));
        diskUsage.append(" MB\n");
        diskUsage.append("Free space: ");
        diskUsage.append(std::to_string((diskInfo.f_bfree * diskInfo.f_frsize) / (1024 * 1024)));
        diskUsage.append(" MB\n");
        diskUsage.append("Used space: ");
        diskUsage.append(std::to_string(((diskInfo.f_blocks - diskInfo.f_bfree) * diskInfo.f_frsize) / (1024 * 1024)));
        diskUsage.append(" MB\n");
        ImGui::Text("%s", diskUsage.c_str());
        ImGui::EndChild();

        ImGui::EndChildFrame();
        
        ImGui::BeginChild("##FileOperations", ImVec2(ImGui::GetWindowWidth() * 0.5f, ImGui::GetTextLineHeight() * 16), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::PushFont(largeFont);
        ImGui::SeparatorText("Directory and File Operations");
        ImGui::PopFont();

        ImGui::Separator();
        // Directory creation
        ImGui::Text("Directory Name");
        ImGui::InputText("##DirectoryName", dirName, IM_ARRAYSIZE(dirName));
        if (ImGui::Button("Create Directory")) {
            result = create_directory(dirName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory created successfully: %s", dirName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error creating directory: %s (%s)", dirName, strerror(result));
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete Directory")) {
            result = delete_directory(dirName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory deleted successfully: %s", dirName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error deleting directory: %s (%s)", dirName, strerror(result));
            }
        }

        // File creation
        ImGui::Text("File Name");
        ImGui::InputText("##File Name", fileName, IM_ARRAYSIZE(fileName));
        if (ImGui::Button("Create File")) {
            if (strlen(dirName) == 0) {
                result = 1;
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Please enter a directory name first");
            } else{
                result = create_file(fileName);
                if (result == 0) {
                    snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File created successfully: %s/%s", dirName, fileName);
                    writingToFile[0] = '\0';  // Clear the previous content
                    // Write the file path to the writingToFile buffer
                    snprintf(writingToFile, IM_ARRAYSIZE(writingToFile), "Writing to file: %s/%s", dirName, fileName);
                } else {
                    snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error creating file: %s (%s)", fileName, strerror(result));
                }
            }
        }

        ImGui::SameLine();
        // delete file
        if (ImGui::Button("Delete File")) {
            result = delete_file(fileName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File deleted successfully: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error deleting file: %s (%s)", fileName, strerror(result));
            }
        }

             // Change File Permissions
        ImGui::Text("File Permissions (octal)");
        ImGui::InputText("##File Permisssions (octal) ", newPermissionsStr, sizeof(newPermissionsStr));
        if (ImGui::Button("Change Permissions##Button")) {
            // Convert the string back to mode_t
            mode_t parsedPermissions = strtol(newPermissionsStr, nullptr, 8);  // Convert octal string to mode_t

            result = change_permissions(fileName, parsedPermissions);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Permissions changed successfully: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error changing permissions: %s", strerror(result));
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##FileOperations2", ImVec2(0, ImGui::GetTextLineHeight() * 16), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        // Write to file
        ImGui::PushFont(largeFont);
        ImGui::SeparatorText("File Content to Read/Write: ");
        ImGui::PopFont();

        ImGui::Text("%s", writingToFile);
        // Render the input field
        ImGui::InputTextMultiline(
            "##FileContent",                 // Label for the input field
            &fileContent[0],                 // Pointer to the writable buffer
            fileContent.size() + 1,          // Buffer size (including null terminator)
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10),  // Size of the input field
            ImGuiInputTextFlags_AllowTabInput 
        );
        if (ImGui::Button("Write to File")) {
            result = write_to_file(fileName, fileContent.c_str());
            if (result == 0) {
            snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Data written to file: %s", fileName);
            } else {
            snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error writing to file: %s (%s)", fileName, strerror(result));
            }
        }

        ImGui::SameLine();
        // Read from file
        if (ImGui::Button("Read File")) {
            std::string content = read_file(fileName);
            if (content.rfind("Error:",0) == 0) {
            snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File read successfully: %s (check console for content)", fileName);
            fileContent[0] = '\0';
            } else {
            snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File read successfully: %s", fileName);
            fileContent = content;
            }
        }

        ImGui::EndChild();
        ImGui::Separator();

         // Begin a resizable child window
        ImGui::BeginChild("##dirContentsChild", ImVec2(0, ImGui::GetTextLineHeight() * 16), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::PushFont(largeFont);
        ImGui::SeparatorText("File Info and Directory Contents");
        ImGui::PopFont();
        // Get file info
        ImGui::InputText("File Path", filePath, IM_ARRAYSIZE(filePath));
        if (ImGui::Button("Get File Info")) {
             struct stat info = get_file_info(filePath);
            dirContents.clear();

            if (info.st_size == -1) {
                dirContents.append("File does not exist: ");
            } else {
                // fill the dirContents with the file info
                dirContents.append("File: ");
                dirContents.append(filePath);
                dirContents.append("\nSize: ");
                dirContents.append(std::to_string(info.st_size));
                dirContents.append(" bytes\nPermissions: ");
                // Display permissions in octal format
                char permissions_tmp[8];
                snprintf(permissions_tmp, sizeof(permissions_tmp), "%o", (info.st_mode & 0777));
                dirContents.append(permissions_tmp);
                dirContents.append("\nLast modified: ");
                dirContents.append(ctime(&info.st_mtime));
                dirContents.append("Last accessed: ");
                dirContents.append(ctime(&info.st_atime));
                dirContents.append("Creation time: ");
                dirContents.append(ctime(&info.st_ctime));
            }
        }

        // Display directory contents if available
        ImGui::InputTextMultiline("##dirContents", &dirContents[0], dirContents.size() + 1, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_ReadOnly | ImGuiWindowFlags_AlwaysVerticalScrollbar);
       
        // List directory contents
        if (ImGui::Button("List Directory Contents")) {
            ImGui::Text("Directory contents for: %s", filePath);
            dirContents = list_directory_contents(filePath);  // Get contents of the specified directory
            if (dirContents.rfind("Error:", 0) == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "%s", dirContents.c_str());
                dirContents.clear();  // Clear if it's an error message
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory contents for: %s", filePath);
            }
        }
        ImGui::SameLine();
        // Adding a dummy item to push the "Clear" button to the right
        if (ImGui::Button("Clear")) {
            dirContents.clear();
        }
        ImGui::EndChild();




        // New Operations: Rename, Move, Copy, Permissions, etc.
        ImGui::BeginChild("##FileModifications", ImVec2(0, ImGui::GetTextLineHeight() * 16), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushFont(largeFont);
        ImGui::SeparatorText("File Modifications");
        ImGui::PopFont();

        // Rename File/Directory
        ImGui::InputText("Old Name (Rename)", oldName, IM_ARRAYSIZE(oldName));
        ImGui::InputText("New Name (Rename)", newName, IM_ARRAYSIZE(newName));
        if (ImGui::Button("Rename")) {
            result = rename_file_or_directory(oldName, newName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Renamed successfully: %s -> %s", oldName, newName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error renaming: %s -> %s", oldName, newName);
            }
        }

        // Move File/Directory
        ImGui::InputText("Source", moveCopySource, IM_ARRAYSIZE(moveCopySource));
        ImGui::InputText("Destination", moveCopyDestination, IM_ARRAYSIZE(moveCopyDestination));
        if (ImGui::Button("Move")) {
            result = move_file_or_directory(moveCopySource, moveCopyDestination);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Moved successfully: %s -> %s", moveCopySource, moveCopyDestination);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error moving: %s -> %s", moveCopySource, moveCopyDestination);
            }
        }

        ImGui::SameLine();

        // Copy File
        if (ImGui::Button("Copy")) {
            result = copy_file(moveCopySource, moveCopyDestination);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Copied successfully: %s -> %s", moveCopySource, moveCopyDestination);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error copying: %s -> %s", moveCopySource, moveCopyDestination);
            }
        }

        ImGui::EndChild();

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


