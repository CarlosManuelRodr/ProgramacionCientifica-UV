using UnityEngine;
using UnityEngine.U2D;

public class JointForceArrow : MonoBehaviour
{
    public Transform obj;
    public HingeJoint2D joint;
    public float scale = 1.0f;

    private SpriteShapeController spriteShapeController;
    private Spline spline;

    // Start is called before the first frame update
    void Start()
    {
        spriteShapeController = GetComponent<SpriteShapeController>();
        spline = spriteShapeController.spline;
    }

    void UpdateArrowPosition()
    {
        Vector2 origin = obj.transform.position - this.transform.position;
        Vector2 end = origin + scale * joint.reactionForce;

        spline.SetPosition(0, origin);
        spline.SetPosition(1, end);
    }

    // Update is called once per frame
    void LateUpdate()
    {
        UpdateArrowPosition();
    }
}
