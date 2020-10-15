using UnityEngine;
using UnityEngine.U2D;

public class FixedForceArrow : MonoBehaviour
{
    public Transform obj;
    public Vector2 direction;
    public float scale = 1.0f;

    private SpriteShapeController spriteShapeController;
    private Spline spline;

    // Start is called before the first frame update
    void Start()
    {
        spriteShapeController = GetComponent<SpriteShapeController>();
        spline = spriteShapeController.spline;

        UpdateArrowPosition();
    }

    void UpdateArrowPosition()
    {
        Vector2 origin = obj.transform.position - this.transform.position;
        Vector2 end = origin + scale * direction;

        spline.SetPosition(0, origin);
        spline.SetPosition(1, end);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        UpdateArrowPosition();
    }
}
