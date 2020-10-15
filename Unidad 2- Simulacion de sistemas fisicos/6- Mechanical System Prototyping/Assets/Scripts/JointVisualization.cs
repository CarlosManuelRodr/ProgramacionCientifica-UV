using UnityEngine;
using UnityEngine.U2D;

public class JointVisualization : MonoBehaviour
{
    public Transform obj1;
    public Transform obj2;

    private SpriteShapeController spriteShapeController;
    private Spline spline;

    // Start is called before the first frame update
    void Start()
    {
        spriteShapeController = GetComponent<SpriteShapeController>();
        spline = spriteShapeController.spline;
    }

    // Update is called once per frame
    void Update()
    {
        spline.SetPosition(0, obj1.position - this.transform.position);
        spline.SetPosition(1, obj2.position - this.transform.position);
    }
}
