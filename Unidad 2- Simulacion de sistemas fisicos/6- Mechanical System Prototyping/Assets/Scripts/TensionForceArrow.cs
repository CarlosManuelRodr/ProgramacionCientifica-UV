using UnityEngine;
using UnityEngine.U2D;

public class TensionForceArrow : MonoBehaviour
{
    public Transform begin;
    public Transform end;
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
        Vector2 direction = end.position - begin.position;
        Vector2 origin = begin.transform.position - this.transform.position;
        Vector2 endpoint = origin + scale * direction;

        spline.SetPosition(0, origin);
        spline.SetPosition(1, endpoint);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        UpdateArrowPosition();
    }
}
