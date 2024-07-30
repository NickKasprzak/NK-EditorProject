using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WinFormsSandbox.Source;

/*
* Gonna have to wrap the engine, game object,
* and components into their own C# interface
* classes. Its gonna be a pain but I kinda have
* to do it. All of these will have to store
* IntPtrs to the unmanaged code to keep it
* managed.
* 
* The engine would wrap whatever is needed as
* time progresses.
* 
* Game Object
* 
* The game object would wrap creation and deletion
* by getting an available game object pointer from
* the engine and deletion would execute the deletion
* process from the engine. The class would also
* need to store references to its components and
* have methods to handle their creation and deletion.
* 
* Component Creation and Deletion
* 
* Creation is simple enough, calling the object's
* create component function and creating a wrapped
* instance of the component from that pointer.
* 
* Deletion is tricky as deleting a component through
* its respective component pool doesn't remove the
* reference to unused space in memory in the game
* object. What we could do is, when deleting the
* component from the TreeView, first just call the
* delete function of that component from the pool as
* usual, then go into the game object associated with
* the parent TreeNode and remove the reference to that
* component and clear the reference to the wrapped
* component instance.
* 
* Or we could step into the parent of the component
* and handle its deletion from there. Like, get the
* engine game object and set its pointer to that
* component to null, then call the component pool's
* deletion function.
* 
* Both of these solutions MIGHT be stupid. Since
* I (hopefully) don't have to worry about the C#
* classes storing a reference to its transform
* anymore, I should be able to get the exported delete
* component function of the game object by passing
* in a pointer to that object from the component
* by getting a pointer to its parent game object.
*/
namespace WinFormsSandbox
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            engine = new Engine();

            Timer tmr = new Timer();
            tmr.Interval = 30;
            tmr.Tick += UpdateTimerTick;
            tmr.Start();

            engine.AttachToPanel(panel1);

            saveToolStripMenuItem.Click += new System.EventHandler(SaveLevelFromDialog);
            loadToolStripMenuItem.Click += new System.EventHandler(LoadLevelFromDialog);
            gameObjectToolStripMenuItem.Click += new System.EventHandler(MenuStripCreateGameObject);
            transformToolStripMenuItem.Click += new System.EventHandler(MenuStripCreateTransform);
            rendererToolStripMenuItem.Click += new System.EventHandler(MenuStripCreateRenderer);
            treeView1.AfterSelect += UpdatePropertyGridWithNode;
            treeView1.KeyDown += new KeyEventHandler(TreeViewButtonPress);
        }

        private void SaveLevelFromDialog(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                engine.SaveLevel(saveFileDialog1.FileName);
            }
        }

        private void LoadLevelFromDialog(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                InitializeLevel(openFileDialog1.FileName);
            }
        }

        private void InitializeLevel(string levelPath)
        {
            engine.LoadLevel(levelPath);

            UpdateTreeView();
        }

        private void UpdateTreeView()
        {
            treeView1.Nodes.Clear();

            List<GameObject> gameObjects = engine.GetGameObjectList();
            for (int i = 0; i < gameObjects.Count; i++)
            {
                TreeNode newNode = treeView1.Nodes.Add("GameObject " + i);
                newNode.Tag = gameObjects[i];

                Transform objTransform = gameObjects[i].GetTransform();
                if (objTransform != null)
                {
                    TreeNode transformNode = newNode.Nodes.Add("Transform");
                    transformNode.Tag = objTransform;
                }

                RectangleRenderer objRenderer = gameObjects[i].GetRenderer();
                if (objRenderer != null)
                {
                    TreeNode rendererNode = newNode.Nodes.Add("Rectangle Renderer");
                    rendererNode.Tag = objRenderer;
                }
            }
        }

        private void UpdatePropertyGridWithNode(object sender, TreeViewEventArgs e)
        {
            propertyGrid1.SelectedObject = treeView1.SelectedNode.Tag;
        }

        private void TreeViewButtonPress(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == (char)Keys.Delete)
            {
                TreeNode selected = treeView1.SelectedNode;

                // Not ideal that I have to handle all deletion scenarios
                // for all object types thanks to how I structured the wrappers.
                // Would be better if all of them were derived from a base
                // managed object super class that had a virtual deletion
                // function that would vary on a type to type basis, but
                // this works for now.

                // Handles the deletion of a game object
                if (selected.Tag.GetType() == typeof(GameObject))
                {
                    GameObject asObj = (GameObject)selected.Tag;
                    asObj.DestroyGameObject();
                    treeView1.SelectedNode.Remove();
                }

                // Handles the deletion of a transform
                if (selected.Tag.GetType() == typeof(Transform))
                {
                    GameObject asObj = (GameObject)selected.Parent.Tag;

                    if (asObj != null)
                    {
                        asObj.RemoveTransform();
                        treeView1.SelectedNode.Remove();
                    }

                }

                if (selected.Tag.GetType() == typeof(RectangleRenderer))
                {
                    GameObject asObj = (GameObject)selected.Parent.Tag;

                    if (asObj != null)
                    {
                        asObj.RemoveRenderer();
                        treeView1.SelectedNode.Remove();
                    }
                }
            }
        }

        private void MenuStripCreateGameObject(object sender, EventArgs e)
        {
            // Create a fully new and empty game object
            // from the game object's default constructor
            // and add it to the tree view for use
            GameObject newObj = new GameObject();
            TreeNode newObjNode = treeView1.Nodes.Add("GameObject");
            newObjNode.Tag = newObj;
        }

        private void MenuStripCreateTransform(object sender, EventArgs e)
        {
            // Call the selected node's game object's create transform function
            // but only if the selected is a game object
            TreeNode selected = treeView1.SelectedNode;
            if (selected == null) { return; }

            if (selected.Tag.GetType() == typeof(GameObject))
            {
                GameObject asObj = (GameObject)selected.Tag;

                if (asObj.GetTransform() == null)
                {
                    asObj.AddTransform();
                    selected.Nodes.Add("Transform").Tag = asObj.GetTransform();
                }
            }
        }

        private void MenuStripCreateRenderer(object sender, EventArgs e)
        {
            TreeNode selected = treeView1.SelectedNode;
            if (selected == null) { return; }

            if (selected.Tag.GetType() == typeof(GameObject))
            {
                GameObject asObj = (GameObject)selected.Tag;

                if (asObj.GetRenderer() == null)
                {
                    asObj.AddRenderer();
                    selected.Nodes.Add("Rectangle Renderer").Tag = asObj.GetRenderer();
                }
            }
        }

        private void UpdateTimerTick(object sender, EventArgs e)
        {
            engine.RunLoop();
        }

        private Engine engine;
    }
}
